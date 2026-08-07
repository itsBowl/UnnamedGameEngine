#include "OpenGLVertexBuffer.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "OpenGL_Vertex_Buffer";

    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t s, GLenum usage)
    {
        create(data, s, usage);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        destory();
    }

    void OpenGLVertexBuffer::create(const void* data, uint32_t s, GLenum usage)
    {
        size = s;
        glGenBuffers(1, &id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        Log::info(LOGGER_TAG, "Created VBO: ", id, " size: ", size);
        unbind();
    }

    void OpenGLVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void OpenGLVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size, uint32_t offset)
    {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    void OpenGLVertexBuffer::destory()
    {
        if (exists())
        {
            Log::info(LOGGER_TAG, "Destorying VBO: ", id);
            glDeleteBuffers(1, &id);
            id = 0;
        }
    }

    
}