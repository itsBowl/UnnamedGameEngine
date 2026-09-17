#pragma once
#include "Buffers/IStructuredBuffer.hpp"
#include <d3d12.h>
#include <wrl/client.h>

namespace EngineCore
{
    class DirectX12StructuredBuffer : public IStructuredBuffer
    {
    public:
        DirectX12StructuredBuffer(ID3D12Device* device, uint32_t maxElements, uint32_t elementStride);
        ~DirectX12StructuredBuffer() override;

        void setData(const void* data, uint32_t count, uint32_t stride);
        uint32_t getElementCount() const override { return elementCount; }

        D3D12_GPU_DESCRIPTOR_HANDLE getHandle() const { return gpuHandle; }

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;

        uint8_t* mappedData = nullptr;
        uint32_t elementCount = 0;
        uint32_t stride = 0;
        uint32_t capacity = 0;

        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {};

        void createHeap(ID3D12Device* device);
    };
}

