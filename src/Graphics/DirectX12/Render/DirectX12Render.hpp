#pragma once
#include "Render/IRender.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>


namespace EngineCore
{
    class Mesh;
    class IShader;
    class DirectX12Shader;

    class DirectX12Render : public IRender
    {
    public:
        void init(const WindowHandle& w) override;
        void shutdown() override;
        void waitForGPU() override { dxWaitForGPU(); }
        void setWindowProps();

        void beginFrame() override;
        void endFrame() override;

        void setClearColour(const glm::vec4& c) override;
        void clear() override;

        void draw(std::vector<std::shared_ptr<Mesh>> m, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo = {}) override;
        void draw(Mesh& m, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo = {}) override;
        void draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo = {}) override;
        void draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> s, std::vector<std::shared_ptr<IUniformBuffer>> ubo = {}, uint32_t indexCount = 0) override;
        void drawArrays(std::shared_ptr<IVertexArray> vao, uint32_t verts, std::vector<std::shared_ptr<IUniformBuffer>> ubo = {}) override;

        void setViewport(int x, int y, int w, int h) override;
        void setPipelineState(const PipelineState& p);
        const PipelineState& getPipelineState() const { return pipelineState; };

        const Stats& getStats() const override { return stats; }
    
    private:
        void createDevice();
        void createCommandQueue();
        void createSwapChain();
        void createDesciptorHeap();
        void createRenderTarget();
        void createDepthStencilBuffer();
        void createCommandAllocatorsList();
        void createFence();

        void dxWaitForGPU();
        void nextFrame();

        //Pipeline state object and caching
        ID3D12PipelineState* getOrCreatePSO(const PipelineState& state, DirectX12Shader* s, std::shared_ptr<IVertexArray> vao);

        struct PSOKey
        {
            PipelineState state;
            DirectX12Shader* shader;
            bool operator==(const PSOKey& other) const
            {
                return state == state && shader == shader;
            }
        };

        struct PSOKeyHash
        {
            size_t operator()(const PSOKey& key) const
            {
                size_t k = std::hash<bool>()(key.state.depthTest)
                    ^ (std::hash<bool>()(key.state.blending) << 1)
                    ^ (std::hash<bool>()(key.state.wireframe) << 2)
                    ^ (std::hash<bool>()(key.state.backfaceCull) << 3);
                size_t l = std::hash<void*>()(static_cast<void*>(key.shader));
                return k ^ (l << 4);
            }
        };

        std::unordered_map<PSOKey, Microsoft::WRL::ComPtr<ID3D12PipelineState>, PSOKeyHash> psoCache;

        static const uint32_t frameCount = 2;
        Stats stats;
        PipelineState pipelineState;
        uint32_t width, height;
        HWND hwnd = nullptr;


        Microsoft::WRL::ComPtr<ID3D12Device> device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
        Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[frameCount];
        Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllcoators[frameCount];
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

        

        Microsoft::WRL::ComPtr<ID3D12Fence> fence;
        uint64_t fenceValues[frameCount];
        HANDLE fenceEvent = nullptr;

        uint32_t renderDescriptorSize = 0;
        uint32_t frameIndex = 0;

        D3D12_VIEWPORT viewport = {};
        D3D12_RECT scissorRect = {};
        glm::vec4 clearColour = glm::vec4(1.f, .4f, .2f, 1.f);
        
    };
}