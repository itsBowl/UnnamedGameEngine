#pragma once
#include <cstdint>


namespace EngineCore
{
    class IStructuredBuffer
    {
    public:
        virtual ~IStructuredBuffer() = default;

        virtual void setData(const void* data, uint32_t count, uint32_t stide) = 0;
        virtual uint32_t getElementCount() const = 0;
    };
}