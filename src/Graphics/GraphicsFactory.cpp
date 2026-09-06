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
//DirectX 12 THIS NEEDS ITS GUARD
#ifdef DX_12_SUPPORT
#include <d3d12.h>
#include "DirectX12/Buffers/DirectX12VertexArray.hpp"
#include "DirectX12/Buffers/DirectX12VertexBuffer.hpp"
#include "DirectX12/Buffers/DirectX12IndexBuffer.hpp"
#include "DirectX12/Buffers/DirectX12UniformBuffer.hpp"
#include "DirectX12/Render/DirectX12Render.hpp"
#include "DirectX12/Shader/DirectX12Shader.hpp"
#endif
//TODO:
// DIRECTX12 GUARD

namespace EngineCore
{
    ID3D12Device* GraphicsFactory::dXdev = nullptr;

    void GraphicsFactory::setDevice(ID3D12Device* d)
    {
        dXdev = d;
    }
    std::unique_ptr<IRender> GraphicsFactory::createRender()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_unique<OpenGLRender>();
            case GraphicsAPI::DirectX12: return std::make_unique<DirectX12Render>();
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<IShader> GraphicsFactory::createShader()
    {
        switch (getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLShader>();
            case GraphicsAPI::DirectX12: return std::make_shared<DirectX12Shader>();
           
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<IShader> GraphicsFactory::createShader(const std::string& fp)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLShader>(fp);
            case GraphicsAPI::DirectX12:
            {
                if (dXdev != nullptr) return std::make_shared<DirectX12Shader>(dXdev, fp);
                Log::fatal("GraphicsFactory", "Factory does not have the correct device");
            } 
        }

        Log::flush();
        return nullptr;
    }

    std::shared_ptr<ITexture> GraphicsFactory::createTexture()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLTexture>();
            case GraphicsAPI::DirectX12: return nullptr;
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<ITexture> GraphicsFactory::createTexture(const std::string& fp)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLTexture>(fp);
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<IVertexArray> GraphicsFactory::createVertexArray()
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLVertexArray>();
            case GraphicsAPI::DirectX12: return std::make_shared<DirectX12VertexArray>();
            //other cases
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<IVertexBuffer> GraphicsFactory::createVertexBuffer(const void* data, uint32_t size)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLVertexBuffer>(data, size);
            case GraphicsAPI::DirectX12:
            {
                if (dXdev != nullptr) return std::make_shared<DirectX12VertexBuffer>(dXdev, data, size);
                Log::fatal("GraphicsFactory", "Factory does not have the correct device");
            } 
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<IIndexBuffer> GraphicsFactory::createIndexBuffer(const uint32_t* indicies, uint32_t count)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indicies, count);
            case GraphicsAPI::DirectX12:
            {
                if (dXdev != nullptr) return std::make_shared<DirectX12IndexBuffer>(dXdev, indicies, count);
                Log::fatal("GraphicsFactory", "Factory does not have the correct device");
            } 
        }
        Log::flush();
        return nullptr;
    }

    std::shared_ptr<IUniformBuffer> GraphicsFactory::createUniformBuffer(uint32_t size)
    {
        switch(getActiveGraphicsAPI())
        {
            case GraphicsAPI::OpenGL: return std::make_shared<OpenGLUniformBuffer>();
            case GraphicsAPI::DirectX12: return std::make_shared<DirectX12UniformBuffer>(dXdev, size);
        }
        Log::flush();
        return nullptr;
    }
}