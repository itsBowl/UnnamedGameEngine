#pragma once
#include <memory>
#include "Buffers/IVertexArray.hpp"
#include "Buffers/IVertexBuffer.hpp"
#include "Buffers/IIndexBuffer.hpp"
#include "Render/IRender.hpp"
#include "Shader/IShader.hpp"

namespace EngineCore
{
    class GraphicsFactory
    {
    public:
        static std::unique_ptr<IRender> createRender();
        static std::shared_ptr<IShader> createShader();
        static std::shared_ptr<IShader> createShader(const std::string& fp);
        static std::shared_ptr<IVertexArray> createVertexArray();
        static std::shared_ptr<IVertexBuffer> createVertexBuffer(const void* data, uint32_t size);
        static std::shared_ptr<IIndexBuffer>  createIndexBuffer (const uint32_t* indices, uint32_t count);
    };
}