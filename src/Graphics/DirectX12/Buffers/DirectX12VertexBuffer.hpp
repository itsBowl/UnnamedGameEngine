#pragma once
#include "IVertexBuffer.hpp"
#include <d3d12.h>
#include <wrl/client.h>
#include <cstdint>

namespace EngineCore
{
    class DirectX12VertexBuffer : public IVertexBuffer
    {
    public:
        DirectX12VertexBuffer(ID3D12Device* device, const void* data, uint32_t size);
        ~DirectX12VertexBuffer() override = default;

        void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

        const D3D12_VERTEX_BUFFER_VIEW& getView() const { return bufferView; }

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
        D3D12_VERTEX_BUFFER_VIEW bufferView = {};
        uint8_t* mappedData = nullptr;
        uint32_t size = 0;
    };
    
}