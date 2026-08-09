#pragma once
#include "Buffers/IVertexArray.hpp"
#include <gl/gl3w.h>

namespace EngineCore
{
    class OpenGLVertexArray : public IVertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;
        void create();
        void bind() const override;
        void unbind() const override;
        void destroy();

        void addVertexBuffer(std::shared_ptr<IVertexBuffer> vbo, const BufferLayout& layout);
        void addIndexBuffer(std::shared_ptr<IIndexBuffer> ibo);

        bool exists() const { return id != 0; }
        uint32_t getIndexCount() const override { return indexCount; }
    private:
        uint32_t id = 0;
        uint32_t attributeIndex = 0;
        uint32_t indexCount = 0;
        std::shared_ptr<IVertexBuffer> vertexBuffer;
        std::shared_ptr<IIndexBuffer> indexBuffer;
    };
}