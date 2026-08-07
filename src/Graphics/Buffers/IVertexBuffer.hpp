#pragma once
#include <cstdint>


namespace EngineCore
{
    class IVertexBuffer
    {
    public:
        virtual ~IVertexBuffer() = default;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
    };
}