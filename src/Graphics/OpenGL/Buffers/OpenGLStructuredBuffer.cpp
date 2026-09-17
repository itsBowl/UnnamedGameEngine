#include "OpenGLStructuredBuffer.hpp"
#include "Log.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "OpenGLStrucutredBuffer";

    OpenGLStructuredBuffer::OpenGLStructuredBuffer()
    {
        glGenBuffers(1, &ssbo);
    }

    OpenGLStructuredBuffer::~OpenGLStructuredBuffer()
    {
        glDeleteBuffers(0, &ssbo);
    }

    void OpenGLStructuredBuffer::setData(const void* data, uint32_t count, uint32_t stride)
    {
        elementCount = count;
        uint32_t requiredBytes = count * stride;

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

        if (requiredBytes > capacityBytes)
        {
            glBufferData(GL_SHADER_STORAGE_BUFFER, requiredBytes, data, GL_DYNAMIC_DRAW);
            capacityBytes = requiredBytes;
        }
        else
        {
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, requiredBytes, data);
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void OpenGLStructuredBuffer::bindToSlot(uint32_t slot) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, ssbo);
    }
}