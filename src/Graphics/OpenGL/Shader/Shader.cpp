#include "Shader.hpp"
#include "FileManagement.hpp"
#include "Errors.hpp"
#include <magic_enum/magic_enum.hpp>

namespace EngineCore
{
    static const std::string LOGGER_TAG = "SHADER";
    void Shader::bind() const
    {
        glUseProgram(id);
    }

    void Shader::unbind() const
    {
        //program 0 is no shader, therefor counts as an unbind call
        glUseProgram(0);
    }

    int Shader::load(const std::string& vp, const std::string& fp)
    {
        id = glCreateProgram();

        std::vector<char> vc;
        std::vector<char> fc;
        namespace FM = FileManagement;
        if (!FM::readFile(vp, &vc)) { return ShaderErrors::SHADER_FAILED_READ_VS; }
        if (!FM::readFile(fp, &fc)) { return ShaderErrors::SHADER_FAILED_READ_FS; }

        auto vs = compileStage(vc, GL_VERTEX_SHADER);
        auto fs = compileStage(fc, GL_FRAGMENT_SHADER);


        
        glAttachShader(id, vs);
        glAttachShader(id, fs);
        glLinkProgram(id);

        int status = checkShaderCompileStatus(id);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return status;
    }

    GLuint Shader::compileStage(const std::vector<char>& src, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        const char* srcPtr = src.data();
        glShaderSource(shader, 1, &srcPtr, nullptr);
        glCompileShader(shader);
        err = checkShaderCompileStatus(shader);
        flushLogs();
        return shader;
    }

    int Shader::checkShaderCompileStatus(GLuint shader)
    {
        GLint res = GL_FALSE;
        ShaderType shaderType = getShaderType(shader);
        int logLen;
        int srcLen;

        if (shaderType != ShaderType::SHADER_TYPE_PROGRAM)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &res);
        }
        logInfo(LOGGER_TAG, "[", magic_enum::enum_name(shaderType), "] for program [", id, "] compile result: ", res);
        if (res == GL_FALSE)
        {
            if (shaderType != ShaderType::SHADER_TYPE_PROGRAM)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
                std::string error((logLen > 1) ? logLen : 1, '\0');
                glGetShaderInfoLog(shader, logLen, NULL, error.data());

                glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &srcLen);
                std::string src((srcLen > 1) ? srcLen : 1, '\0');
                glGetShaderSource(shader, srcLen, NULL, src.data());

                logError(LOGGER_TAG, src, "\nError compiling shader!\n\n", error);
                return ShaderErrors::SHADER_FAILED_COMPILE;
            }
            else
            {
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLen);
                std::string error((logLen > 1 ) ? logLen : 1, '\0');
                glGetProgramInfoLog(shader, logLen, NULL, error.data());
                logError(LOGGER_TAG, "Error linking program!\n\n", error);
                return ShaderErrors::SHADER_FAILED_LINK;
            }
        }

        return ShaderErrors::SHADER_OK;
    }

    Shader::ShaderType Shader::getShaderType(GLuint shader)
    {
        int type;
        if (glIsShader(shader))
        {
            glGetShaderiv(shader, GL_SHADER_TYPE, &type);
        }
        else
        {
            type = GL_PROGRAM;
        }
        return static_cast<ShaderType>(type);
    }
}