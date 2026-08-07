#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "IShader.hpp"

typedef unsigned int GLenum;

namespace EngineCore
{
    class ShaderLibrary
    {
    public:
        void add(const std::shared_ptr<IShader>&);
        void add(const std::string&, const std::shared_ptr<IShader>&);

        std::shared_ptr<IShader> load(const std::string&);
        std::shared_ptr<IShader> load(const std::string&, const std::string&);

        std::shared_ptr<IShader> get(const std::string&);

        bool exists(const std::string&) const;

        void deleteAll();

        void debugPrintShaders();

    private:
        std::unordered_map<std::string, std::shared_ptr<IShader>> shaders;
    };
}