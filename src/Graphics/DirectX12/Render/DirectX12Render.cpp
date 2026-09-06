#include "DirectX12Render.hpp"
#include "DirectX12/Shader/DirectX12Shader.hpp"
#include "DirectX12/Buffers/DirectX12VertexArray.hpp"
#include "DirectX12/Buffers/DirectX12UniformBuffer.hpp"
#include "DirectX12/Buffers/DirectX12DataTypes.hpp"
#include "Asset/Mesh/Mesh.hpp"
#include "Graphics/GraphicsFactory.hpp"

using Microsoft::WRL::ComPtr;

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DX12 Render";


    void DirectX12Render::init(const WindowHandle& h)
    {
        hwnd = static_cast<HWND>(h.handle);
        width = h.width;
        height = h.height;

        createDevice();
        createCommandQueue();
        createSwapChain();
        createDesciptorHeap();
        createRenderTarget();
        createDepthStencilBuffer();
        createCommandAllocatorsList();
        createFence();

        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width    = static_cast<float>(width);
        viewport.Height   = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        scissorRect.left   = 0;
        scissorRect.top    = 0;
        scissorRect.right  = static_cast<LONG>(width);
        scissorRect.bottom = static_cast<LONG>(height);

        Log::info(LOGGER_TAG, "DirectX12Render initialised, ", width, "x", height);
        Log::flush();

    }

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
        GraphicsFactory::setDevice(device.Get());

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
        factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
        if (FAILED(res))
        {
            Log::fatal(LOGGER_TAG, "Failed to create swap chain");
            return;
        }

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

        renderDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        //Log::info(LOGGER_TAG, "Created render target desciptor heap.");

        D3D12_DESCRIPTOR_HEAP_DESC dsDesc = {};
        dsDesc.NumDescriptors = 1;
        dsDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        HRESULT dsRes = device->CreateDescriptorHeap(&dsDesc, IID_PPV_ARGS(&depthDescriptorHeap));

        if (FAILED(dsRes))
        {
            Log::fatal(LOGGER_TAG, "Failed to create depth stencil view descriptor heap");
            return;
        }

        Log::info(LOGGER_TAG, "Created depth, stencil and render target heaps.");
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
        clear.DepthStencil.Stencil = 0;

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
            Log::fatal(LOGGER_TAG, "Failed to create depth/stencil buffer HRESULT=0x", std::hex, res);
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

        if (fenceEvent == nullptr)
        {
            Log::fatal(LOGGER_TAG, "Failed to create fence event");
            return;
        }

        Log::info(LOGGER_TAG, "Create fence");
    }

    void DirectX12Render::dxWaitForGPU()
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

    void DirectX12Render::beginFrame()
    {
        HRESULT resetResult = commandAllcoators[frameIndex]->Reset();
        if (FAILED(resetResult))
        {
            Log::error(LOGGER_TAG, "Failed to reset command allocator");
            return;
        }

        resetResult = commandList->Reset(commandAllcoators[frameIndex].Get(), nullptr);
        if (FAILED(resetResult))
        {
            Log::error(LOGGER_TAG, "Failed to reset command list");
            return;
        }

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = renderTargets[frameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->ResourceBarrier(1, &barrier);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = renderDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += static_cast<SIZE_T>(frameIndex) * renderDescriptorSize;

        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        
        commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
        setViewport(0, 0, width, height);
    }

    void DirectX12Render::clear()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = renderDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += static_cast<SIZE_T>(frameIndex) * renderDescriptorSize;

        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        const float clear[4] = {clearColour.r, clearColour.g, clearColour.b, clearColour.a};
        commandList->ClearRenderTargetView(rtvHandle, clear, 0, nullptr);
        commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
    }

    void DirectX12Render::endFrame()
    {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = renderTargets[frameIndex].Get();
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->ResourceBarrier(1, &barrier);

        HRESULT res = commandList->Close();

        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to close command list");
            return;
        }

        ID3D12CommandList* lists[] = {commandList.Get()};
        
        commandQueue->ExecuteCommandLists(1, lists);

        res = swapChain->Present(1, 0);
        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to present swap chain");
            return;
        }
        stats.drawCalls = 0;
        stats.indexCount = 0;
        Log::info(LOGGER_TAG, "End of frame");
        Log::flush();
        nextFrame();
    }

    void DirectX12Render::shutdown()
    {
        Log::info(LOGGER_TAG, "Shutdown");
    }

    void DirectX12Render::setClearColour(const glm::vec4& c)
    {
        clearColour = c;
    }

    void DirectX12Render::setViewport(int x, int y, int w, int h)
    {
        viewport.TopLeftX = static_cast<float>(x);
        viewport.TopLeftY = static_cast<float>(y);
        viewport.Width = static_cast<float>(w);
        viewport.Height = static_cast<float>(h);
        viewport.MaxDepth = 1.f;
        viewport.MinDepth = 0.f;

        scissorRect.left = x;
        scissorRect.top = y;
        scissorRect.right = x + w;
        scissorRect.bottom = y + h;

        commandList->RSSetViewports(1, &viewport);
        commandList->RSSetScissorRects(1, &scissorRect);
        Log::info(LOGGER_TAG, "Set viewport: ", x, " ", y, " ", w, " ", h);
    }

    ID3D12PipelineState* DirectX12Render::getOrCreatePSO(const PipelineState& state, DirectX12Shader* s, std::shared_ptr<IVertexArray> _vao)
    {
        PSOKey k{state, s};

        std::unordered_map<PSOKey, ComPtr<ID3D12PipelineState>, PSOKeyHash>::iterator it = psoCache.find(k);
        if (it != psoCache.end())
        {
            return it->second.Get();
        }

        //PSO creation
        //This was moved from the DX12VA object as string lifetime was causing errors
        //With SemanticName setting
        DirectX12VertexArray* vao = static_cast<DirectX12VertexArray*>(_vao.get());

        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
        uint32_t inputSlot = static_cast<uint32_t>(vao->getVertexBufferViews().size() - 1);
        if (inputSlot > 1000) { inputSlot = 0; }
        BufferLayout layout = vao->getBufferLayout();
        for (const BufferElement& e: layout.getElements())
        {
            D3D12_INPUT_ELEMENT_DESC d = {};
            d.SemanticName = e.name.c_str();
            d.SemanticIndex = 0;//static_cast<UINT>(inputLayout.size());
            d.Format = shaderDataTypeToDX12(e.type);
            d.InputSlot = inputSlot;
            d.AlignedByteOffset = e.offset;
            d.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            d.InstanceDataStepRate = 0;
            inputLayout.push_back(d);
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = {inputLayout.data(), static_cast<UINT>(inputLayout.size())};
        psoDesc.pRootSignature = s->getRootSignature();
        psoDesc.VS = s->getVertexBytecode();
        psoDesc.PS = s->getFragmentBytecode();
        psoDesc.RasterizerState = {};
        psoDesc.RasterizerState.FillMode = state.wireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.CullMode = state.backfaceCull ? D3D12_CULL_MODE_BACK : D3D12_CULL_MODE_FRONT;
        psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
        psoDesc.BlendState = {};
        psoDesc.BlendState.RenderTarget[0].BlendEnable = state.blending ? TRUE : FALSE;
        psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        psoDesc.BlendState.RenderTarget[1].BlendEnable = state.blending ? TRUE : FALSE;
        if (state.blending)
        {
            psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
            psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
            psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
            psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
            psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
            psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        }

        psoDesc.DepthStencilState.DepthEnable = state.depthTest ? TRUE : FALSE;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        psoDesc.SampleDesc.Count = 1;

        ComPtr<ID3D12PipelineState> pso;
        HRESULT res = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));
        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to create pipelien state object for shader: ", s->getName());
            Log::flush();
            return nullptr;
        }

        ID3D12PipelineState* ptr = pso.Get();
        psoCache[k] = pso;
        return ptr;
    }

    void DirectX12Render::setPipelineState(const PipelineState& p)
    {
        if (p == pipelineState)
            return;
        pipelineState = p;        
    }

    void DirectX12Render::draw(std::vector<std::shared_ptr<Mesh>> m, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo)
    {
        for (std::shared_ptr<Mesh> _m : m)
        {
            draw(_m, s, ubo);
        }
    }

    void DirectX12Render::draw(Mesh& m, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo)
    {
        DirectX12Shader* shader = static_cast<DirectX12Shader*>(s.get());

        ID3D12PipelineState* pso = getOrCreatePSO(getPipelineState(), shader, m.getVAO());

        commandList->SetPipelineState(pso);
        commandList->SetGraphicsRootSignature(shader->getRootSignature());
        

        for (uint32_t i = 0; i < ubo.size(); i++)
        {
            DirectX12UniformBuffer* u = static_cast<DirectX12UniformBuffer*>(ubo[i].get());
            commandList->SetGraphicsRootConstantBufferView(i, u->getGPUAddress());
        }

        draw(m.getVAO(), s, ubo);
    }

    void DirectX12Render::draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubos)
    {
        DirectX12Shader* shader = static_cast<DirectX12Shader*>(s.get());

        ID3D12PipelineState* pso = getOrCreatePSO(getPipelineState(), shader, m.get()->getVAO());

        commandList->SetPipelineState(pso);
        ID3D12RootSignature* rs = shader->getRootSignature();
        commandList->SetGraphicsRootSignature(shader->getRootSignature());

        for (uint32_t i = 0; i < ubos.size(); i++)
        {
            DirectX12UniformBuffer* u = static_cast<DirectX12UniformBuffer*>(ubos[i].get());
            commandList->SetGraphicsRootConstantBufferView(i, u->getGPUAddress());
        }

        draw(m->getVAO(), s, ubos);
    }

    void DirectX12Render::draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo, uint32_t indexCount)
    {
        DirectX12VertexArray* array = static_cast<DirectX12VertexArray*>(vao.get());
        const std::vector<D3D12_VERTEX_BUFFER_VIEW>& views = array->getVertexBufferViews();

        commandList->IASetVertexBuffers(0, static_cast<UINT>(views.size()), views.data());
        commandList->IASetIndexBuffer(&array->getIndexBufferView());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        uint32_t count = (indexCount == 0) ? vao->getIndexCount() : indexCount;

        commandList->DrawIndexedInstanced(count, 1, 0, 0, 0);

        stats.drawCalls++;
        stats.indexCount += count;
    }

    void DirectX12Render::drawArrays(std::shared_ptr<IVertexArray> vao, uint32_t verts, std::vector<std::shared_ptr<IUniformBuffer>> ubo)
    {
        const std::vector<D3D12_VERTEX_BUFFER_VIEW>& views = static_cast<DirectX12VertexArray*>(vao.get())->getVertexBufferViews();

        for (uint32_t i = 0; i < ubo.size(); i++)
        {
            commandList->SetGraphicsRootConstantBufferView(i,
                static_cast<DirectX12UniformBuffer*>(ubo[i].get())->getGPUAddress());
        }

        commandList->IASetVertexBuffers(0, static_cast<UINT>(views.size()), views.data());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        commandList->DrawInstanced(verts, 1, 0, 0);
        stats.drawCalls++;
    }

}