#include "DirectX12IndexBuffer.hpp"
#include "Log.hpp"

using Microsoft::WRL::ComPtr;

namespace EngineCore
{

    static const std::string LOGGER_TAG = "DirectX12 Index Buffer";

    DirectX12IndexBuffer::DirectX12IndexBuffer(ID3D12Device* device, const uint32_t* idx, uint32_t count) : count(count)
    {
        uint32_t bufferSize = count * sizeof(uint32_t);

        D3D12_HEAP_PROPERTIES heap = {};
        heap.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC resource = {};
        resource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resource.Width = bufferSize;
        resource.Height = 1;
        resource.DepthOrArraySize = 1;
        resource.MipLevels = 1;
        resource.Format = DXGI_FORMAT_UNKNOWN;
        resource.SampleDesc.Count = 1;
        resource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        HRESULT resourceRes = device->CreateCommittedResource(
            &heap,
            D3D12_HEAP_FLAG_NONE,
            &resource,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&buffer)
        );

        if (FAILED(resourceRes))
        {
            Log::error(LOGGER_TAG, "Failed to create index buffer resource: ", count);
            return;
        }

        uint8_t* mappedData = nullptr;
        D3D12_RANGE readRange = {0, 0};
        HRESULT mapRes = buffer->Map(0, &readRange, reinterpret_cast<void**>(&mappedData));
        if (FAILED(mapRes))
        {
            Log::error(LOGGER_TAG, "Failed to map index buffer");
            return;
        }

        memcpy(mappedData, idx, bufferSize);
        buffer->Unmap(0, nullptr);

        bufferView.BufferLocation = buffer->GetGPUVirtualAddress();
        bufferView.SizeInBytes = bufferSize;
        bufferView.Format = DXGI_FORMAT_R32_UINT;

        Log::info(LOGGER_TAG, "Created index buffer, Sz: ", count);
    }
}