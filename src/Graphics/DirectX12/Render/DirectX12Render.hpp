#pragma once
#include "IRender.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>


namespace EngineCore
{
    class Mesh;
    class IShader;

    class DirectX12Render : public IRender
    {
    public:
        void init(const WindowHandle& w) override;
        void shutdown() override;
        void setWindowProps();

        void beginFrame() override;
        void endFrame() override;

        void setClearColour(const glm::vec4& c) override;
        void clear() override;

        void draw(Mesh& m, std::shared_ptr<IShader> s) override;
        void draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> s) override;
        void draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> s, uint32_t indexCount = 0) override;
        void drawArrays(std::shared_ptr<IVertexArray>, uint32_t verts) override;

        void setViewport(int x, int y, int w, int h) override;
        void setPipelineState(const PipelineState& p);
        const PipelineState& getPipelineState() const;

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

        void waitForGPU();
        void nextFrame();

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