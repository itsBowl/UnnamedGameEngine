#include "DirectX12VertexArray.hpp"
#include "DirectX12IndexBuffer.hpp"
#include "DirectX12VertexBuffer.hpp"
#include "DirectX12DataTypes.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DirectX12 Vertex Array";

    void DirectX12VertexArray::addVertexBuffer(std::shared_ptr<IVertexBuffer> vbo, const BufferLayout& layout)
    {
        D3D12_VERTEX_BUFFER_VIEW view = static_cast<DirectX12VertexBuffer*>(vbo.get())->getView();

        view.StrideInBytes = layout.getStride();
        vertexBufferViews.push_back(view);

        uint32_t inputSlot = static_cast<uint32_t>(vertexBufferViews.size() - 1);

        for (const BufferElement& e: layout.getElements())
        {
            D3D12_INPUT_ELEMENT_DESC d = {};
            d.SemanticName = e.name.c_str();
            d.SemanticIndex = static_cast<UINT>(inputLayout.size());
            d.Format = shaderDataTypeToDX12(e.type);
            d.InputSlot = inputSlot;
            d.AlignedByteOffset = e.offset;
            d.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            d.InstanceDataStepRate = 0;

            inputLayout.push_back(d);
        }

        vertexBuffers.push_back(vbo);
    }

    void DirectX12VertexArray::addIndexBuffer(std::shared_ptr<IIndexBuffer> ibo)
    {
        indexBufferView = static_cast<DirectX12IndexBuffer*>(ibo.get())->getView();
        indexCount = ibo->getCount();
        indexBuffer = ibo;
    }
}