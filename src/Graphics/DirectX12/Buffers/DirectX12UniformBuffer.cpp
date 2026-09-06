#include "DirectX12UniformBuffer.hpp"
#include "Log.hpp"



using Microsoft::WRL::ComPtr;

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DirectX12 Uniform Buffer";

    DirectX12UniformBuffer::DirectX12UniformBuffer()
    {
        Log::error(LOGGER_TAG, "Creating a DirectX12 buffer without paramters causes issues, don't do this");
    }

    DirectX12UniformBuffer::DirectX12UniformBuffer(ID3D12Device* d, uint32_t size)
    {
        uint32_t as256 = (size + 255) & ~255;

        D3D12_HEAP_PROPERTIES heap = {};
        heap.Type = D3D12_HEAP_TYPE_UPLOAD;
        D3D12_RESOURCE_DESC resDesc = {};
        resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resDesc.Width = as256;
        resDesc.Height = 1;
        resDesc.DepthOrArraySize = 1;
        resDesc.MipLevels = 1;
        resDesc.Format = DXGI_FORMAT_UNKNOWN;
        resDesc.SampleDesc.Count = 1;
        resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        HRESULT res = d->CreateCommittedResource(
            &heap,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource)
        );

        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to create constant buffer size: ", as256);
            return;
        }

        D3D12_RANGE read = {0, 0};
        res = resource->Map(0, &read, reinterpret_cast<void**>(&mappedData));

        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to map buffer");
            return;
        }

        Log::info(LOGGER_TAG, "Created constant buffer size: ", as256);
    }

    DirectX12UniformBuffer::~DirectX12UniformBuffer()
    {
        if (resource && mappedData)
        {
            resource->Unmap(0, nullptr);
        }
    }

    void DirectX12UniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
    {
        if (mappedData == nullptr)
        {
            Log::error(LOGGER_TAG, "called on unmapped data");
            return;
        }

        memcpy(mappedData + offset, data, size);
    }
}