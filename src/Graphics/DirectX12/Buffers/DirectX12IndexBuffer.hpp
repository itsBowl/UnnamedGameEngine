#pragma once
#include "Buffers/IIndexBuffer.hpp"
#include <d3d12.h>
#include <wrl/client.h>
#include <cstdint>

namespace EngineCore
{
    class DirectX12IndexBuffer : public IIndexBuffer
    {
    public:
        DirectX12IndexBuffer(ID3D12Device* device, const uint32_t* idx, uint32_t count);
        ~DirectX12IndexBuffer() override = default;

        uint32_t getCount() const override { return count; }

        const D3D12_INDEX_BUFFER_VIEW& getView() const {return bufferView; }

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
        D3D12_INDEX_BUFFER_VIEW bufferView = {};
        uint32_t count = 0;
    };
}