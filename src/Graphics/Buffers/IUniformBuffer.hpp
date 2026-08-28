#pragma once
#include <cstdint>

namespace EngineCore
{
    class IUniformBuffer
    {
    public:
        virtual ~IUniformBuffer() = default;

        virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
        
    };
}