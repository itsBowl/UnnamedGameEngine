#include "Mesh.hpp"
#include "Graphics/GraphicsFactory.hpp"

namespace EngineCore
{
    void Mesh::create(const std::vector<Vertex>& verts, const std::vector<uint32_t>& idx)
    {
        vao = GraphicsFactory::createVertexArray();
        //this did use auto, but since we're using an interface class
        //we don't need auto.
        //Safer
        std::shared_ptr<IVertexBuffer> _vbo = GraphicsFactory::createVertexBuffer(verts.data(), (uint32_t)(verts.size() * sizeof(Vertex)));
        std::shared_ptr<IIndexBuffer> _ibo = GraphicsFactory::createIndexBuffer(idx.data(), (uint32_t)idx.size());

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };

        vao->addVertexBuffer(_vbo, layout);
        vao->addIndexBuffer(_ibo);
        vbo = _vbo;
        ibo = _ibo;

    }

    void Mesh::bind() const
    {
        vao->bind();
    }
}