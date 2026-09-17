#pragma once
#include "Graphics/Shader/ShaderLib.hpp"
#include "Graphics/Texture/TextureLib.hpp"
#include "Mesh/MeshLibrary.hpp"

namespace EngineCore
{
    class AssetManager
    {
    public:
        ShaderLibrary& shader() { return shaders; }
        TextureLibrary& texture() { return textures; }
        MeshLibrary& mesh() { return meshes; }
        void clearAssets()
        {
            shaders.deleteAll();
            textures.deleteAll();
            meshes.deleteAll();
        }

    private:
        ShaderLibrary shaders;
        TextureLibrary textures;
        MeshLibrary meshes;
    };
}