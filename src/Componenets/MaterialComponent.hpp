#pragma once
#include <memory>
#include <Shader/IShader.hpp>
#include <Texture/ITexture.hpp>

namespace EngineCore
{
    struct MaterialComponent
    {
        std::shared_ptr<IShader> shader;
        std::shared_ptr<ITexture> texture;
    };
}