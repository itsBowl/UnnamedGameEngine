#pragma once
#include "PCH.hpp"
#include "Errors/Errors.hpp"
#include "gl/gl3w.h"
#include <glm/gtc/type_ptr.hpp>
namespace EngineCore{


class Shader
{
public:
    int load(const std::string& vp, const std::string& fp);
    void bind() const;
    void unbind() const;

    void setInt(const std::string& name, int v) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (GLint)v);
    }
    void setFloat(const std::string& name, float v) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), GLfloat(v));
    }
    void setVec2(const std::string& name, glm::vec2 v) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
    }
    void setVec4(const std::string& name, glm::vec4 v) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
    }
    void setMat4(const std::string& name, glm::mat4 v) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
    }

private:
//GL_FRAGMENT_SHADER                0x8B30
//GL_VERTEX_SHADER                  0x8B31
    enum ShaderType
    {
        SHADER_TYPE_NONE = 0,
        SHADER_TYPE_FRAGMENT = 0x8B30,
        SHADER_TYPE_VERTEX = 0x8B31,
        SHADER_TYPE_PROGRAM = 0x82E2
    };
    GLuint id;
    ShaderError err;
    GLuint compileStage(const std::vector<char>& src, GLenum type);
    int checkShaderCompileStatus(GLuint shader);
    ShaderType getShaderType(GLuint shader);
};

}