#pragma once
#include <memory>
#include "Buffers/IVertexArray.hpp"
#include "Buffers/IVertexBuffer.hpp"
#include "Buffers/IIndexBuffer.hpp"
#include "Render/IRender.hpp"
#include "Shader/IShader.hpp"
#include "Texture/ITexture.hpp"
#include "Buffers/IUniformBuffer.hpp"

struct ID3D12Device;

namespace EngineCore
{
    

    class GraphicsFactory
    {
    public:

        //API specific setters for internal usage
        static void setDevice(ID3D12Device* d);

        static std::unique_ptr<IRender> createRender();

        static std::shared_ptr<IShader> createShader();
        static std::shared_ptr<IShader> createShader(const std::string& fp);
        
        static std::shared_ptr<ITexture> createTexture();
        static std::shared_ptr<ITexture> createTexture(const std::string& fp);
        
        static std::shared_ptr<IVertexArray> createVertexArray();
        static std::shared_ptr<IVertexBuffer> createVertexBuffer(const void* data, uint32_t size);
        static std::shared_ptr<IIndexBuffer>  createIndexBuffer (const uint32_t* indices, uint32_t count);
        static std::shared_ptr<IUniformBuffer> createUniformBuffer(uint32_t size = 0);

    private:
        static ID3D12Device* dXdev;
    };
}