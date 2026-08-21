#include "TextureLib.hpp"
#include "Graphics/GraphicsFactory.hpp"


namespace EngineCore
{
    static const std::string LOGGER_TAG = "Texture Library";

    void TextureLibrary::add(const std::string& name, const std::shared_ptr<ITexture>& t)
    {
        if (exists(name)) { Log::warn(LOGGER_TAG, "Texture already exists", name); return;}
        textures[name] = t;
    }

    void TextureLibrary::add(const std::shared_ptr<ITexture>& t)
    {
        add(t->getName(), t);
    }

    std::shared_ptr<ITexture> TextureLibrary::load(const std::string& path)
    {
        if (exists(path))
            return get(path);
            
        std::shared_ptr<ITexture> tex = GraphicsFactory::createTexture(path);
        if (!tex)
        {
            Log::error(LOGGER_TAG, "Failed to laod texture: ", path);
            return nullptr;
        }
        add(tex);
        return tex;
    }

    std::shared_ptr<ITexture> TextureLibrary::get(const std::string& name)
    {
        std::unordered_map<std::string, std::shared_ptr<ITexture>>::const_iterator it = textures.find(name);
        if (it == textures.end())
        {
            Log::error(LOGGER_TAG, "Texture ", name, "doesn't exist");
            return nullptr;
        }
        return it->second;
    }

    bool TextureLibrary::exists(const std::string& name) const
    {
        return textures.find(name) != textures.end();
    }

    void TextureLibrary::debug()
    {
        for(std::pair<std::string, std::shared_ptr<ITexture>> t : textures)
        {
            std::shared_ptr<ITexture> tex = t.second;
            Log::info(LOGGER_TAG, tex.get()->getName());
        }
        Log::flush();
    }
}