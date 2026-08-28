#pragma once
#include "PCH.hpp"
#include "Buffers/IIndexBuffer.hpp"
#include "GL/gl3w.h"


namespace EngineCore
{
    class OpenGLIndexBuffer : public IIndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* idx, uint32_t count, GLenum usage = GL_STATIC_DRAW);
        ~OpenGLIndexBuffer() override;
        void create(const uint32_t* idx, uint32_t count, GLenum usage = GL_STATIC_DRAW);
        void bind() const;
        void unbind() const;
        void destory();
        uint32_t getCount() const override { return count; }
        bool exists() const { return id != 0; }
    private:
        uint32_t id = 0;
        uint32_t count = 0;
    };
}