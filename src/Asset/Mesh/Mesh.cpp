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
        vbo = GraphicsFactory::createVertexBuffer(verts.data(), (uint32_t)(verts.size() * sizeof(Vertex)));
        ibo = GraphicsFactory::createIndexBuffer(idx.data(), (uint32_t)idx.size());

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };

        vao->addVertexBuffer(vbo, layout);
        vao->addIndexBuffer(ibo);
    }

}