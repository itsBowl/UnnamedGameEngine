#pragma once
#include "PCH.hpp"
#include "Buffers/IVertexBuffer.hpp"
#include "GL/gl3w.h"

namespace EngineCore
{
    class OpenGLVertexBuffer : public IVertexBuffer
    {
    public:
        OpenGLVertexBuffer(const void* data, uint32_t size, GLenum usage = GL_STATIC_DRAW);
        ~OpenGLVertexBuffer() override;
        void create(const void* data, uint32_t s, GLenum usage = GL_STATIC_DRAW);
        void bind() const;
        void unbind() const;
        void destory();

        void setData(const void* data, uint32_t size, uint32_t offset = 0);
        uint32_t getID() const {return id; }
        bool exists() const { return id != 0; }
    private:
        GLuint id = 0;
        uint32_t size;
    };

    

    
}