#include "OpenGLVertexBuffer.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Vertex Buffer";
    void VertexBuffer::create(const void* data, uint32_t s, GLenum usage)
    {
        size = s;
        glGenBuffers(1, &id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        logInfo(LOGGER_TAG, "Created VBO: ", id, " size: ", size);
        unbind();
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void* data, uint32_t size, uint32_t offset)
    {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    void VertexBuffer::destory()
    {
        if (exists())
        {
            logInfo(LOGGER_TAG, "Destorying VBO: ", id);
            glDeleteBuffers(1, &id);
            id = 0;
        }
    }

    
}