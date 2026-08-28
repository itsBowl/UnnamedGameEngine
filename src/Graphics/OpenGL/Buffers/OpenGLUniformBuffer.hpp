#pragma once
#include "Buffers/IUniformBuffer.hpp"
#include "gl/gl3w.h"

namespace EngineCore
{
    class OpenGLUniformBuffer : public IUniformBuffer
    {
    public:
        OpenGLUniformBuffer();
        OpenGLUniformBuffer(const void* data, uint32_t size, uint32_t offset = 0);
        ~OpenGLUniformBuffer() override;
        void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
        
        void bindToSlot(uint32_t slot) const;
        
    private:
        GLuint ubo = 0;
        void createUBO(uint32_t size);
    };
}