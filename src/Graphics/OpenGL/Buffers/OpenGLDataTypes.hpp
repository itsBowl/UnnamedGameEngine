#pragma once
#include "Buffers/BufferLayout.hpp"
#include <GL/gl3w.h>

namespace EngineCore
{
    static GLenum shaderDataTypeOpenGL(ShaderDataType t)
    {
        switch (t)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
        }
        return GL_NONE;
    }
}