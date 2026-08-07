#include "GraphicsFactory.hpp"
#include "GraphicsAPI.hpp"
#include "OpenGL/Buffers/OpenGLVertexArray.hpp"
#include "OpenGL/Buffers/OpenGLVertexBuffer.hpp"
#include "OpenGL/Buffers/OpenGLIndexBuffer.hpp"
#include "OpenGL/Render/OpenGLRender.hpp"
#include "OpenGL/Shader/OpenGLShader.hpp"

namespace EngineCore
{
    std::unique_ptr<IRender> GraphicsFactory::createRender()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_unique<OpenGLRender>();
        }
        return nullptr;
    }

    std::shared_ptr<IShader> GraphicsFactory::createShader()
    {
        switch (getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLShader>();
        }

        return nullptr;
    }

    std::shared_ptr<IShader> GraphicsFactory::createShader(const std::string& fp)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLShader>(fp);
        }

        return nullptr;
    }

    std::shared_ptr<IVertexArray> GraphicsFactory::createVertexArray()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLVertexArray>();
            //other cases
        }
        return nullptr;
    }

    std::shared_ptr<IVertexBuffer> GraphicsFactory::createVertexBuffer(const void* data, uint32_t size)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLVertexBuffer>(data, size);
        }
        return nullptr;
    }

    std::shared_ptr<IIndexBuffer> GraphicsFactory::createIndexBuffer(const uint32_t* indicies, uint32_t count)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indicies, count);
        }
        return nullptr;
    }
}