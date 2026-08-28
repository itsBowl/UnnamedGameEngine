#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Errors/Errors.hpp"

namespace EngineCore
{
    class IShader
    {
    public:
        virtual ~IShader() = default;

        //virtual int load(const std::string& vp, const std::string& fp) = 0;
        //virtual void bind() const = 0;
        //virtual void unbind() const = 0;

        virtual ShaderErrors getError() const = 0;
        virtual const std::string& getName() const = 0;

        //virtual void setInt(const std::string& name, int v) const = 0;
        //virtual void setFloat(const std::string& name, float v) const = 0;
        //virtual void setVec2(const std::string& name, glm::vec2 v) const = 0;
        //virtual void setVec3(const std::string& name, glm::vec3 v) const = 0;
        //virtual void setVec4(const std::string& name, glm::vec4 v) const = 0;
        //virtual void setMat4(const std::string& name, glm::mat4 v) const = 0;
    };
}