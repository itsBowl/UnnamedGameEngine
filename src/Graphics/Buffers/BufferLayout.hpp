#pragma once
#include "PCH.hpp"

namespace EngineCore
{
    enum class ShaderDataType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Bool,
    };

    static uint32_t shaderDataTypeSize(ShaderDataType t)
    {
        switch (t)
        {
            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Int:    return 4;
            case ShaderDataType::Int2:   return 4 * 2;
            case ShaderDataType::Int3:   return 4 * 3;
            case ShaderDataType::Int4:   return 4 * 4;
            case ShaderDataType::Bool:   return 1;
        }
        return 0;
    }

    static uint32_t shaderDataTypeComponentCount(ShaderDataType t)
    {
        switch (t)
        {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
        }
        return 0;
    }

    struct BufferElement
    {
        std::string name;
        ShaderDataType type;
        uint32_t size;
        uint32_t offset = 0;
        bool normalised = false;

        BufferElement(ShaderDataType t, const std::string& n, bool norm = false):
        name(n), type(t), size(shaderDataTypeSize(t)), normalised(norm)
        {}
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> e):
        elements(e)
        {
            calculateOffsetStride();
        };

        const std::vector<BufferElement>& getElements() const { return elements; }
        uint32_t getStride() const { return stride; }

    private:
        void calculateOffsetStride()
        {
            uint32_t offset = 0;
            stride = 0;
            for (auto& e : elements)
            {
                e.offset = offset;
                offset += e.size;
                stride += e.size;
            }
        }

        std::vector<BufferElement> elements;
        uint32_t stride = 0;
    };
}