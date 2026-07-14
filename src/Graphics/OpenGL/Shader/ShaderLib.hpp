#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Shader.hpp"

typedef unsigned int GLenum;

namespace EngineCore
{
    class ShaderLibrary
    {
    public:
        void add(const std::shared_ptr<Shader>&);
        void add(const std::string&, const std::shared_ptr<Shader>&);

        std::shared_ptr<Shader> load(const std::string&);
        std::shared_ptr<Shader> load(const std::string&, const std::string&);

        std::shared_ptr<Shader> get(const std::string&);

        bool exists(const std::string&) const;

        void debugPrintShaders();

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    };
}