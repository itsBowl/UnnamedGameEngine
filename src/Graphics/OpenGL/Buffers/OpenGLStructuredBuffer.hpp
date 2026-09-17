#pragma once
#include "Buffers/IStructuredBuffer.hpp"
#include <GL/gl3w.h>

namespace EngineCore
{
    class OpenGLStructuredBuffer : public IStructuredBuffer
    {
    public:
        OpenGLStructuredBuffer();
        ~OpenGLStructuredBuffer();

        void setData(const void* data, uint32_t count, uint32_t stide) override;
        uint32_t getElementCount() const override { return elementCount; }

        void bindToSlot(uint32_t slot) const;

    private:
        uint32_t ssbo = 0;
        uint32_t elementCount = 0;
        uint32_t capacityBytes = 0;
    };
}