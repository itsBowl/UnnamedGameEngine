#pragma once
#include "PCH.hpp"
#include "OpenGLVertexBuffer.hpp"
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLBufferLayout.hpp"

namespace EngineCore
{
    class VertexArray
    {
    public:
        void create();
        void bind() const;
        void unbind() const;
        void destroy();

        void addVertexBuffer(const VertexBuffer& vbo, const BufferLayout& layout);
        void setIndexBuffer(const IndexBuffer& ibo);

        bool exists() const { return id != 0; }
        bool getIndexCount() const { return indexCount; }
    private:
        uint32_t id = 0;
        uint32_t attributeIndex = 0;
        uint32_t indexCount = 0;
    };
}