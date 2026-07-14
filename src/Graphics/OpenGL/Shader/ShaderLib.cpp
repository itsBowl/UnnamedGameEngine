#include "PCH.hpp"
#include "ShaderLib.hpp"
#include "Shader.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "SHADER_LIBRARY";

    void ShaderLibrary::add(const std::string& name, const std::shared_ptr<Shader>& shader)
    {
        if (exists(name)) {logError(LOGGER_TAG, "Shader already exists"); return;}
        shaders[name] = shader;
    }
    
    void ShaderLibrary::add(const std::shared_ptr<Shader>& shader)
    {
        auto& name = shader->getName();
        add(name, shader);
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string& fp)
    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(fp);
        add(shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string& name, const std::string& fp)
    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(fp);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::get(const std::string& name)
    {
        if (!exists(name)) {logError(LOGGER_TAG, "shader ", name, " doesn't exist!"); return nullptr;}
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
            logInfo(LOGGER_TAG, shd.get()->getName());
        }
        flushLogs();
    }
}