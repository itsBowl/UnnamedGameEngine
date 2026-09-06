#pragma once
#include "Buffers/IUniformBuffer.hpp"
#include <wrl/client.h>
#include <d3D12.h>

namespace EngineCore
{
    class DirectX12UniformBuffer : public IUniformBuffer
    {
    public:

        DirectX12UniformBuffer();
        DirectX12UniformBuffer(ID3D12Device*, uint32_t size);
        ~DirectX12UniformBuffer();
        void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

        D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress() const { return resource->GetGPUVirtualAddress(); }

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        uint8_t* mappedData = nullptr;
        uint32_t bindingSlot = 0;
    };
}