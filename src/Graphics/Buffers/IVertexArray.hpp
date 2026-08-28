#pragma once
#include "BufferLayout.hpp"

namespace EngineCore
{
    class IVertexBuffer;
    class IIndexBuffer;

    class IVertexArray
    {
    public:
        virtual ~IVertexArray() = default;

        virtual void addVertexBuffer(std::shared_ptr<IVertexBuffer> vbo, const BufferLayout& layout) = 0;
        virtual void addIndexBuffer(std::shared_ptr<IIndexBuffer> ibo) = 0;

        virtual uint32_t getIndexCount() const = 0;
    };
}