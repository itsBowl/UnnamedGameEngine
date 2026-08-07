#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Buffers/IVertexArray.hpp"

namespace EngineCore
{
    struct Vertex
    {
        alignas(4)glm::vec3 position;
        alignas(4)glm::vec3 normal;
        alignas(4)glm::vec2 uv;
    };

    class Mesh
    {
    public:
        void create(const std::vector<Vertex>& verts, const std::vector<uint32_t>& idx);
        void bind() const;
        std::shared_ptr<IVertexArray> getVAO() { return vao; }
        uint32_t getIndexCount() const { return vao->getIndexCount();}

    private:
        std::shared_ptr<IVertexArray> vao;
        std::shared_ptr<IVertexBuffer> vbo;
        std::shared_ptr<IIndexBuffer> ibo;
    };
}