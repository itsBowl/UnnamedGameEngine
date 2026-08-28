#include "GraphicsFactory.hpp"
#include "GraphicsAPI.hpp"
//OpenGL
#include "OpenGL/Buffers/OpenGLVertexArray.hpp"
#include "OpenGL/Buffers/OpenGLVertexBuffer.hpp"
#include "OpenGL/Buffers/OpenGLIndexBuffer.hpp"
#include "OpenGL/Buffers/OpenGLUniformBuffer.hpp"
#include "OpenGL/Render/OpenGLRender.hpp"
#include "OpenGL/Shader/OpenGLShader.hpp"
#include "OpenGL/Texture/OpenGLTexture.hpp"
//DirectX 12
//#include "DirectX12/Render/DirectX12Render.hpp"

namespace EngineCore
{
    std::unique_ptr<IRender> GraphicsFactory::createRender()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_unique<OpenGLRender>();
            //case GraphicsAPI::DirectX12: return std::make_uniqie<DirectX12Renderer>();
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

    std::shared_ptr<ITexture> GraphicsFactory::createTexture()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLTexture>();
        }
        return nullptr;
    }

    std::shared_ptr<ITexture> GraphicsFactory::createTexture(const std::string& fp)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLTexture>(fp);
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

    std::shared_ptr<IUniformBuffer> GraphicsFactory::createUniformBuffer()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLUniformBuffer>();
        }
    }
}