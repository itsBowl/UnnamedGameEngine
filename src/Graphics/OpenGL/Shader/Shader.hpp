#pragma once
#include <string>
#include <vector>
#include "Errors/Errors.hpp"
#include "gl/gl3w.h"
#include "PCH.hpp"

namespace EngineCore{


class Shader
{
public:
    Shader() = default;
    Shader(const std::string&);
    Shader(const std::string&, const std::string&, const std::string&);
    int load(const std::string& vp, const std::string& fp);
    void bind() const;
    void unbind() const;
    inline const std::string getName() const {return name; }

    void setInt(const std::string& name, int v) const;
    void setFloat(const std::string& name, float v) const;
    void setVec2(const std::string& name, glm::vec2 v) const;
    void setVec4(const std::string& name, glm::vec4 v) const;
    void setMat4(const std::string& name, glm::mat4 v) const;

private:
//GL_FRAGMENT_SHADER                0x8B30
//GL_VERTEX_SHADER                  0x8B31
    enum ShaderType
    {
        SHADER_TYPE_NONE = 0,
        SHADER_TYPE_PROGRAM = 0x1,
        SHADER_TYPE_FRAGMENT = 0x8B30,
        SHADER_TYPE_VERTEX = 0x8B31,
    };
    GLuint id;
    ShaderErrors shaderErrorCode;
    std::string name;
    GLuint compileStage(const std::vector<char>& src, GLenum type);
    ShaderErrors checkShaderCompileStatus(GLuint shader);
    ShaderType getShaderType(GLuint shader);
};

}