#include "OpenGLShader.hpp"
#include "PCH.hpp"
#include "FileManagement.hpp"
#include "Errors.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace EngineCore
{
    static const std::string LOGGER_TAG = "OpenGLShader";

    OpenGLShader::OpenGLShader(const std::string& fp)
    {
        name = fp.substr(fp.find_last_of('/') + 1);
        namespace FM = FileManagement;
        std::vector<std::pair<std::string, std::string>> fps;
        if (!FM::getFilesInFolder(&fps, fp))
        {
            Log::error(LOGGER_TAG, "Failed to load shader from: ", fp);
            return;
        }
        
        //do some filepath size checks here if wanted

        std::string frag, vert;

        for (auto& e : fps)
        {
            if (e.second == ".frag") frag = e.first;
            else if (e.second == ".vert") vert = e.first;
        }

        int ret = load(vert, frag);
        if (ret != ShaderErrors::SHADER_OK)
        {
            Log::error(LOGGER_TAG, "OpenGLShader error in loading");
        }
    }

    ShaderErrors OpenGLShader::getError() const { return error; }

    const std::string& OpenGLShader::getName() const { return name;}

    void OpenGLShader::setInt(const std::string& name, int v) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (GLint)v);
    }
    void OpenGLShader::setFloat(const std::string& name, float v) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), GLfloat(v));
    }
    void OpenGLShader::setVec2(const std::string& name, glm::vec2 v) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
    }
    void OpenGLShader::setVec4(const std::string& name, glm::vec4 v) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
    }
    void OpenGLShader::setMat4(const std::string& name, glm::mat4 v) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
    }

    

    void OpenGLShader::bind() const
    {
        glUseProgram(id);
    }

    void OpenGLShader::unbind() const
    {
        //program 0 is no shader, therefor counts as an unbind call
        glUseProgram(0);
    }

    int OpenGLShader::load(const std::string& vp, const std::string& fp)
    {
        id = glCreateProgram();

        std::vector<char> vc;
        std::vector<char> fc;
        namespace FM = FileManagement;
        if (!FM::readFile(vp, &vc)) { return ShaderErrors::SHADER_FAILED_READ_VS; }
        if (!FM::readFile(fp, &fc)) { return ShaderErrors::SHADER_FAILED_READ_FS; }

        vc.push_back('\0');
        fc.push_back('\0');
        auto vs = compileStage(vc, GL_VERTEX_SHADER);
        if (error != ShaderErrors::SHADER_OK)
        {
            Log::error(LOGGER_TAG, "vertex shader failed");
            glDeleteShader(vs);
            return error;
        }
        auto fs = compileStage(fc, GL_FRAGMENT_SHADER);
        if (error != ShaderErrors::SHADER_OK)
        {
            Log::error(LOGGER_TAG, "fragment shader failed");
            glDeleteShader(vs);
            glDeleteShader(fs);
            return error;
        }


        
        glAttachShader(id, vs);
        glAttachShader(id, fs);
        glLinkProgram(id);

        error = checkShaderCompileStatus(id);
        if (error != ShaderErrors::SHADER_OK)
        {
            Log::error(LOGGER_TAG, "Link Failed");
            return error;
        }
        glDeleteShader(vs);
        glDeleteShader(fs);

        return ShaderErrors::SHADER_OK;
    }

    GLuint OpenGLShader::compileStage(const std::vector<char>& src, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        const char* srcPtr = src.data();
        glShaderSource(shader, 1, &srcPtr, nullptr);
        glCompileShader(shader);
        checkShaderCompileStatus(shader);
        Log::flush();
        if (error != ShaderErrors::SHADER_OK)
        {
            return 0;
        }
        return shader;
    }

    ShaderErrors OpenGLShader::checkShaderCompileStatus(GLuint shader)
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
        Log::info(LOGGER_TAG, "[", magic_enum::enum_name(shaderType), "] for program [", id, "] compile result: ", res);
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

                Log::error(LOGGER_TAG, "\n", src, "\nError compiling shader!\n\n", error);
                return ShaderErrors::SHADER_FAILED_COMPILE;
            }
            else
            {
                if (!glIsProgram(shader))
                {
                    Log::error(LOGGER_TAG, "PROGARM CECK ON NON PROGRAM");
                }
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLen);
                Log::error(LOGGER_TAG, "Log Length: ", logLen);
                std::string error((logLen > 1 ) ? logLen : 1, '\0');
                glGetProgramInfoLog(shader, logLen, NULL, error.data());
                Log::error(LOGGER_TAG, "Error linking program!\n\n", error);
                return ShaderErrors::SHADER_FAILED_LINK;
            }
        }
        Log::info(LOGGER_TAG, "Shader state compliled correctly!");
        return ShaderErrors::SHADER_OK;
    }

    OpenGLShader::ShaderType OpenGLShader::getShaderType(GLuint shader)
    {
        if (glIsProgram(shader))
            return ShaderType::SHADER_TYPE_PROGRAM;

        else if (glIsShader(shader))
        {
            int type;
            glGetShaderiv(shader, GL_SHADER_TYPE, &type);
            return static_cast<ShaderType>(type);
        }

        Log::error(LOGGER_TAG, "getShaderType called with invalid id=", shader);
        return ShaderType::SHADER_TYPE_NONE;
    }
}