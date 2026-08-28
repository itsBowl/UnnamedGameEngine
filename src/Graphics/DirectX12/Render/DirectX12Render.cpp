#include "DirectX12Render.hpp"

using Microsoft::WRL::ComPtr;

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DX12 Render";

    void DirectX12Render::createDevice()
    {
#ifdef _DEBUG
        ComPtr<ID3D12Debug> debugController;
        HRESULT res = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
        if (SUCCEEDED(res))
        {
            debugController->EnableDebugLayer();
        }
#endif

        ComPtr<IDXGIFactory6> factory;
        UINT factoryFlags = 0;
#ifdef _DEBUG
        factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        HRESULT factoryRes = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));
        if (FAILED(factoryRes))
        {
            Log::fatal(LOGGER_TAG, "Failed to create DXGI Factory");
            Log::flush();
            return;
        }

        ComPtr<IDXGIAdapter1> adapter;
        bool adapterFound = false;

        for (UINT i = 0; 
            factory->EnumAdapterByGpuPreference(
                i, 
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, 
                IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;
            i++
        )
        {
            DXGI_ADAPTER_DESC1 d;
            adapter->GetDesc1(&d);
            if (d.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }

            HRESULT createResult = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr);
            if (SUCCEEDED(createResult))
            {
                adapterFound = true;
                break;
            }
        }

        if (!adapterFound)
        {
            Log::fatal(LOGGER_TAG, "No suitable D3D12 device found");
        }

        HRESULT deviceRes = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
        if (FAILED(deviceRes))
        {
            Log::fatal(LOGGER_TAG, "Failed to create D3D12 device");
            Log::flush();
            return;
        }

        DXGI_ADAPTER_DESC1 chosen;
        adapter->GetDesc1(&chosen);
        Log::info(LOGGER_TAG, "D3D12 device created on adapter");
        Log::flush();
    }

    void DirectX12Render::createCommandQueue()
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        HRESULT res = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
        if (FAILED(res))
        {
            Log::fatal(LOGGER_TAG, "Failed to create command queue");
            return;
        }

        Log::info(LOGGER_TAG, "Created command queue");
    }

    void DirectX12Render::createSwapChain()
    {
        ComPtr<IDXGIFactory6> factory;
        UINT factoryFlags = 0;

#ifdef _DEBUG
        factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

        DXGI_SWAP_CHAIN_DESC1 scDesc = {};

        scDesc.BufferCount = frameCount;
        scDesc.Width = width;
        scDesc.Height = height;
        scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> tSC;
        HRESULT res = factory->CreateSwapChainForHwnd(
            commandQueue.Get(),
            hwnd,
            &scDesc,
            nullptr, nullptr,
            &tSC
        );

        if (FAILED(res))
        {
            Log::fatal(LOGGER_TAG, "Failed to create swap chain");
            return;
        }

        factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

        HRESULT cast = tSC.As(&swapChain);
        if (FAILED(cast))
        {
            Log::fatal(LOGGER_TAG, "Failed to cast swap chain to SwapChain3");
            return;
        }

        frameIndex = swapChain->GetCurrentBackBufferIndex();
        Log::info(LOGGER_TAG, "Swap chain started @ frame ", frameIndex);
    }

    void DirectX12Render::createDesciptorHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtDesc = {};
        rtDesc.NumDescriptors = frameCount;
        rtDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        HRESULT rtRes = device->CreateDescriptorHeap(&rtDesc, IID_PPV_ARGS(&renderDescriptorHeap));
        if (FAILED(rtRes))
        {
            Log::fatal(LOGGER_TAG, "Failed to create render target view descriptor heap");
            return;
        }

        Log::info(LOGGER_TAG, "Created render target desciptor heap.");
    }

    void DirectX12Render::createRenderTarget()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtHandle = renderDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        for (uint32_t i = 0; i < frameCount; i++)
        {
            HRESULT res = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));

            if (FAILED(res))
            {
                Log::fatal(LOGGER_TAG, "Failed to get swapchain ", i, " buffer");
                return;
            }
            device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtHandle);
            rtHandle.ptr += static_cast<size_t>(renderDescriptorSize);
        }

        Log::info(LOGGER_TAG, "Created render target views for buffers");
    }

    void DirectX12Render::createDepthStencilBuffer()
    {
        D3D12_HEAP_PROPERTIES props = {};
        props.Type = D3D12_HEAP_TYPE_DEFAULT;

        D3D12_RESOURCE_DESC depth = {};
        depth.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depth.Width = width;
        depth.Height = height;
        depth.DepthOrArraySize = 1;
        depth.MipLevels = 1;
        depth.Format = DXGI_FORMAT_D32_FLOAT;
        depth.SampleDesc.Count = 1;
        depth.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE clear = {};
        clear.Format = DXGI_FORMAT_D32_FLOAT;
        clear.DepthStencil.Depth = 1.f;
        clear.DepthStencil.Stencil = 0.f;

        HRESULT res = device->CreateCommittedResource(
            &props,
            D3D12_HEAP_FLAG_NONE,
            &depth,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &clear,
            IID_PPV_ARGS(&depthStencilBuffer)
        );

        if (FAILED(res))
        {
            Log::fatal(LOGGER_TAG, "Failed to create depth/stencil buffer");
        }

        D3D12_DEPTH_STENCIL_VIEW_DESC dsDesc = {};
        dsDesc.Format = DXGI_FORMAT_D32_FLOAT;
        dsDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

        device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsDesc, depthDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

        Log::info(LOGGER_TAG, "Created depth/stencil buffer");
    }

    void DirectX12Render::createCommandAllocatorsList()
    {
        for (uint32_t i = 0; i < frameCount; i++)
        {
            HRESULT allocRes = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllcoators[i]));
            if (FAILED(allocRes))
            {
                Log::fatal("Failed to create command allocator ", i);
                return;
            }
        }

        HRESULT listRes = device->CreateCommandList(
            0, D3D12_COMMAND_LIST_TYPE_DIRECT,
            commandAllcoators[frameIndex].Get(),
            nullptr,
            IID_PPV_ARGS(&commandList)
        );

        if (FAILED(listRes))
        {
            Log::fatal(LOGGER_TAG, "Failed to create command list");
            return;
        }

        commandList->Close();

        Log::info(LOGGER_TAG, "Created command allocator lists");
    }

    void DirectX12Render::createFence()
    {
        HRESULT res = device->CreateFence(fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

        if (FAILED(res))
        {
            Log::fatal(LOGGER_TAG, "Failed to create fence");
            return;
        }

        fenceValues[frameIndex]++;

        fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        if (fenceEvent = nullptr)
        {
            Log::fatal(LOGGER_TAG, "Failed to create fence event");
            return;
        }

        Log::info(LOGGER_TAG, "Create fence");
    }

    void DirectX12Render::waitForGPU()
    {
        HRESULT sig = commandQueue->Signal(fence.Get(), fenceValues[frameIndex]);
        if (FAILED(sig))
        {
            Log::error(LOGGER_TAG, "Failed to signal fence @ waitForGPU");
            return;
        }

        HRESULT wait = fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent);
        if (FAILED(wait))
        {
            Log::error(LOGGER_TAG, "Failed to set fence completion event");
            return;
        }

        WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);

        fenceValues[frameIndex]++;
    }

    void DirectX12Render::nextFrame()
    {
        const uint64_t currentFence = fenceValues[frameIndex];

        HRESULT signal = commandQueue->Signal(fence.Get(), currentFence);
        if (FAILED(signal))
        {
            Log::error(LOGGER_TAG, "Failed to signal fence @ nextFrame");
            return;
        }

        frameIndex = swapChain->GetCurrentBackBufferIndex();

        if (fence->GetCompletedValue() < fenceValues[frameIndex])
        {
            HRESULT wait = fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent);
            if (FAILED(wait))
            {
                Log::error(LOGGER_TAG, "Failed to set fence completion event @ nextFrame");
                return;
            }
            WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
        }

        fenceValues[frameIndex] = currentFence + 1;
    }

}