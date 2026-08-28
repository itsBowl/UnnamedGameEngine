#include "DirectX12VertexBuffer.hpp"
#include <string>
#include "Log.hpp"

using Microsoft::WRL::ComPtr;

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DirectX12 Vertex Buffer";

    DirectX12VertexBuffer::DirectX12VertexBuffer(ID3D12Device* device, const void* data, uint32_t size) : size(size)
    {
        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = size;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        HRESULT res = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&buffer)
        );

        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to create vertex buffer resource: ", size);
            return;
        }

        D3D12_RANGE readRange = {0, 0};
        HRESULT mapRes = buffer->Map(0, &readRange, reinterpret_cast<void**>(&mappedData));

        if (FAILED(mapRes))
        {
            Log::error(LOGGER_TAG, "Failed to map vertex buffer");
            return;
        }

        if (data != nullptr)
        {
            memcpy(mappedData, data, size);
        }

        bufferView.BufferLocation = buffer->GetGPUVirtualAddress();
        bufferView.SizeInBytes = size;
        bufferView.StrideInBytes = 0;

        Log::info(LOGGER_TAG, "Created vertex buffer: Sz = ", size);
    }

    void DirectX12VertexBuffer::setData(const void* data, uint32_t dataSize, uint32_t offset)
    {
        if (mappedData = nullptr)
        {
            Log::error(LOGGER_TAG, "Called set on unmapped data");
            return;
        }

        memcpy(mappedData + offset, data, dataSize);        
    }
}