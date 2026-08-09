#pragma once
#include <cstdint>

namespace EngineCore
{
    class IIndexBuffer
    {
    public:
        virtual ~IIndexBuffer() = default;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual uint32_t getCount() const = 0;
    };
}