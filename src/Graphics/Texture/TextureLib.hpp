#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Texture/ITexture.hpp"

namespace EngineCore
{
    class TextureLibrary
    {
    public:
        void add(const std::shared_ptr<ITexture>&);
        void add(const std::string&, const std::shared_ptr<ITexture>&);

        std::shared_ptr<ITexture> load(const std::string&);

        std::shared_ptr<ITexture> get(const std::string&);
        bool exists(const std::string&) const;

        void debug();
    private:
        std::unordered_map<std::string, std::shared_ptr<ITexture>> textures;
    };
}