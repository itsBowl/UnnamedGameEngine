#include "OpenGLUniformBuffer.hpp"
#include "glm/glm.hpp"


namespace EngineCore
{
    OpenGLUniformBuffer::OpenGLUniformBuffer()
    {
    }

    OpenGLUniformBuffer::OpenGLUniformBuffer(const void* data, uint32_t size, uint32_t offset)
    {
        setData(data, size, offset);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &ubo);
    }

    void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
    {
        if (ubo == 0)
        {
            createUBO(size);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUniformBuffer::createUBO(uint32_t size)
    {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    void OpenGLUniformBuffer::bindToSlot(uint32_t slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, ubo);
    }
}