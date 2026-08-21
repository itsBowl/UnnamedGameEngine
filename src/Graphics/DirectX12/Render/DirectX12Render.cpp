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

        
    }
}