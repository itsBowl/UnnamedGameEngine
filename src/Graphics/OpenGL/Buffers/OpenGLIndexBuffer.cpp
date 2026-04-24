#include "OpenGLIndexBuffer.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Index Buffer";

    void IndexBuffer::create(const uint32_t* idx, uint32_t c, GLenum usage)
    {
        count = c;
        glGenBuffers(1, &id);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), idx, usage);
        logInfo(LOGGER_TAG, "Created IBO: ", id, " count: ", count);
        unbind();
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::destory()
    {
        if (exists())
        {
            logInfo(LOGGER_TAG, "Destorying IBO: ", id);
            glDeleteBuffers(1, &id);
            id = 0;
        }
        
    }
}