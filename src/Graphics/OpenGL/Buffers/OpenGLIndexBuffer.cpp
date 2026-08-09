#include "OpenGLIndexBuffer.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Index Buffer";

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* idx, uint32_t c, GLenum usage)
    {
        create(idx, c, usage);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        destory();
    }

    void OpenGLIndexBuffer::create(const uint32_t* idx, uint32_t c, GLenum usage)
    {
        count = c;
        glGenBuffers(1, &id);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), idx, usage);
        Log::info(LOGGER_TAG, "Created IBO: ", id, " count: ", count);
        unbind();
    }

    void OpenGLIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void OpenGLIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLIndexBuffer::destory()
    {
        if (exists())
        {
            Log::info(LOGGER_TAG, "Destorying IBO: ", id);
            glDeleteBuffers(1, &id);
            id = 0;
        }
        
    }
}