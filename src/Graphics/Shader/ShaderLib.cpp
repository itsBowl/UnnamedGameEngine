#include "PCH.hpp"
#include "ShaderLib.hpp"
#include "IShader.hpp"
#include "GraphicsFactory.hpp"


namespace EngineCore
{
    static const std::string LOGGER_TAG = "SHADER_LIBRARY";

    void ShaderLibrary::add(const std::string& name, const std::shared_ptr<IShader>& shader)
    {
        if (exists(name)) {Log::warn(LOGGER_TAG, "Shader already exists"); return;}
        shaders[name] = shader;
    }
    
    void ShaderLibrary::add(const std::shared_ptr<IShader>& shader)
    {
        add(shader->getName(), shader);
    }

    std::shared_ptr<IShader> ShaderLibrary::load(const std::string& fp)
    {
        std::cout << "Loading a shader\n";
        std::shared_ptr<IShader> shader = GraphicsFactory::createShader(fp);
        ShaderErrors err = shader->getError();
        if (err != ShaderErrors::SHADER_OK)
        {
            return nullptr;
        }
        add(shader);
        return shader;
    }

    std::shared_ptr<IShader> ShaderLibrary::load(const std::string& name, const std::string& fp)
    {
        if (exists(fp))
            return get(fp);
        std::shared_ptr<IShader> shader = GraphicsFactory::createShader(fp);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<IShader> ShaderLibrary::get(const std::string& name)
    {
        std::unordered_map<std::string, std::shared_ptr<IShader>>::const_iterator it = shaders.find(name);
        if (it == shaders.end())
        {
            Log::error(LOGGER_TAG, "Shader ", name, "doesn't exist");
            return nullptr;
        }
        return it->second;
    }

    bool ShaderLibrary::exists (const std::string& name) const
    {
        return shaders.find(name) != shaders.end();
    }

    void ShaderLibrary::debugPrintShaders()
    {
        for (std::pair<std::string, std::shared_ptr<IShader>> s : shaders)
        {
            std::shared_ptr<IShader> shd = s.second;
            Log::info(LOGGER_TAG, shd.get()->getName());
        }
        Log::flush();
    }

    void ShaderLibrary::deleteAll()
    {
        
    }
}