#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Buffers/IVertexArray.hpp"
#include "Asset/Vertex.hpp"

namespace EngineCore
{


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