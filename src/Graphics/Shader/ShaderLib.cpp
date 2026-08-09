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
        auto& name = shader->getName();
        add(name, shader);
    }

    std::shared_ptr<IShader> ShaderLibrary::load(const std::string& fp)
    {
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
        std::shared_ptr<IShader> shader = GraphicsFactory::createShader(fp);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<IShader> ShaderLibrary::get(const std::string& name)
    {
        if (!exists(name)) {Log::error(LOGGER_TAG, "shader ", name, " doesn't exist!"); return nullptr;}
        return shaders[name];
    }

    bool ShaderLibrary::exists (const std::string& name) const
    {
        return shaders.find(name) != shaders.end();
    }

    void ShaderLibrary::debugPrintShaders()
    {
        for (auto s : shaders)
        {
            auto shd = s.second;
            Log::info(LOGGER_TAG, shd.get()->getName());
        }
        Log::flush();
    }

    void ShaderLibrary::deleteAll()
    {
        
    }
}