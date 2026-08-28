#pragma once
#include <cstdint>

namespace EngineCore
{
    class IIndexBuffer
    {
    public:
        virtual ~IIndexBuffer() = default;
        virtual uint32_t getCount() const = 0;
    };
}