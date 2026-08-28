#pragma once
#include "IVertexArray.hpp"
#include <d3d12.h>
#include <vector>
#include <memory>

namespace EngineCore
{
    class DirectX12VertexArray : public IVertexArray
    {
    public:
        DirectX12VertexArray() = default;
        ~DirectX12VertexArray() override = default;

        void addVertexBuffer(std::shared_ptr<IVertexBuffer> vbo, const BufferLayout& layout) override;
        void addIndexBuffer(std::shared_ptr<IIndexBuffer> ibo) override;

        uint32_t getIndexCount() const override { return indexCount; }

        const std::vector<D3D12_VERTEX_BUFFER_VIEW> getVertexBufferViews() const { return vertexBufferViews; }
        const D3D12_INDEX_BUFFER_VIEW& getIndexBufferView() const { return indexBufferView; }
        const std::vector<D3D12_INPUT_ELEMENT_DESC>& getInputLayout() const { return inputLayout; }

    private:
        std::vector<std::shared_ptr<IVertexBuffer>> vertexBuffers;
        std::shared_ptr<IIndexBuffer> indexBuffer;

        std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferViews;
        D3D12_INDEX_BUFFER_VIEW indexBufferView;
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

        uint32_t indexCount = 0;
    };
}