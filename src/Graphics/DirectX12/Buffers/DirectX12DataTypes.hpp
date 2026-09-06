#pragma once
#include "Buffers/BufferLayout.hpp"
#include <dxgi.h>

namespace EngineCore
{
    static DXGI_FORMAT shaderDataTypeToDX12(ShaderDataType t)
    {
        switch (t)
        {
            case ShaderDataType::Float: return DXGI_FORMAT_R32_FLOAT;
            case ShaderDataType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
            case ShaderDataType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
            case ShaderDataType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case ShaderDataType::Int: return DXGI_FORMAT_R32_SINT;
            case ShaderDataType::Int2: return DXGI_FORMAT_R32G32_SINT;
            case ShaderDataType::Int3: return DXGI_FORMAT_R32G32B32_SINT;
            case ShaderDataType::Int4: return DXGI_FORMAT_R32G32B32A32_SINT;
            case ShaderDataType::Bool: return DXGI_FORMAT_R8_UINT;
        }
        return DXGI_FORMAT_UNKNOWN;
    }
}