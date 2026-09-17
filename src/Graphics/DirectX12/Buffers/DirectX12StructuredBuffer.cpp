#include "DirectX12StrcutredBuffer.hpp"
#include "Log.hpp"


using Microsoft::WRL::ComPtr;

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DirectX12 Structured Buffer";

    DirectX12StructuredBuffer::DirectX12StructuredBuffer(ID3D12Device* device, uint32_t maxElements, uint32_t elementStride)
        : stride(elementStride)
    {
        capacity = maxElements * elementStride;

        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC resourceDesc = {};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Width = capacity;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        HRESULT res = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource)
        );

        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to create structured buffer resource.");
            return;
        }

        D3D12_RANGE readRange = {0, 0};
        res = resource->Map(0, &readRange, reinterpret_cast<void**>(&mappedData));

        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to map structured buffer");
            return;
        }

        createHeap(device);

        Log::info(LOGGER_TAG, "Created structured buffer max: ", maxElements);
    }

    DirectX12StructuredBuffer::~DirectX12StructuredBuffer()
    {
        if (resource && mappedData)
        {
            resource->Unmap(0, nullptr);
        }
    }

    void DirectX12StructuredBuffer::createHeap(ID3D12Device* device)
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = 1;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        HRESULT res = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
        if (FAILED(res))
        {
            Log::error(LOGGER_TAG, "Failed to create descriptor heap");
            return;
        }

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = capacity / stride;
        srvDesc.Buffer.StructureByteStride = stride;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

        device->CreateShaderResourceView(resource.Get(), &srvDesc, heap->GetCPUDescriptorHandleForHeapStart());

        gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
    }

    void DirectX12StructuredBuffer::setData(const void* data, uint32_t count, uint32_t stride)
    {
        uint32_t req = count * stride;

        if (req > capacity)
        {
            Log::error(LOGGER_TAG, "set data too big for buffer");
            return;
        }

        elementCount = count;
        memcpy(mappedData, data, req);
    }
}