#pragma once
#include "Shader/IShader.hpp"
#include <vector>
#include "gl/gl3w.h"

namespace EngineCore{


class OpenGLShader : public IShader
{
public:
    OpenGLShader() = default;
    OpenGLShader(const std::string&);
    //OpenGLShader(const std::string&, const std::string&, const std::string&);
    int load(const std::string& vp, const std::string& fp) override;
    void bind() const override;
    void unbind() const override;
    ShaderErrors getError() const override;
    const std::string& getName() const override;

    void setInt(const std::string& name, int v) const override;
    void setFloat(const std::string& name, float v) const override;
    void setVec2(const std::string& name, glm::vec2 v) const override;
    void setVec4(const std::string& name, glm::vec4 v) const override;
    void setMat4(const std::string& name, glm::mat4 v) const override;

private:
    //GL_FRAGMENT_SHADER                0x8B30
    //GL_VERTEX_SHADER                  0x8B31
    //The Program being 0x1 is kindof messy but it works
    //The frag and vert UID's are taken from OpenGL
    enum ShaderType
    {
        SHADER_TYPE_NONE = 0,
        SHADER_TYPE_PROGRAM = 0x1,
        SHADER_TYPE_FRAGMENT = 0x8B30,
        SHADER_TYPE_VERTEX = 0x8B31,
    };
    GLuint id = 0;
    ShaderErrors error = ShaderErrors::SHADER_OK;
    std::string name = "";
    GLuint compileStage(const std::vector<char>& src, GLenum type);
    ShaderErrors checkShaderCompileStatus(GLuint shader);
    ShaderType getShaderType(GLuint shader);
};

}