#include "PCH.hpp"
#include "OpenGLContext.hpp"
#include "Logging/Logger2.hpp"
#include "Errors/Errors.hpp"




static void openGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		default: return "";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		default: return "";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		default: return "";
		}
	}();

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            logWarn("OpenGLNotification", EngineCore::stringify(src_str, ", ", type_str, ", ", severity_str, ", 0x", std::hex, id, ": ", message, '\n'));
            flushLogs();
            break;
        case GL_DEBUG_SEVERITY_LOW:
            logWarn("OpenGLLow", EngineCore::stringify(src_str, ", ", type_str, ", ", severity_str, ", 0x", std::hex, id, ": ", message, '\n'));
            flushLogs();
            break;
		case GL_DEBUG_SEVERITY_MEDIUM:
            logWarn("OpenGLMedium", EngineCore::stringify(src_str, ", ", type_str, ", ", severity_str, ", 0x", std::hex, id, ": ", message, '\n'));
            flushLogs();
            break;
		case GL_DEBUG_SEVERITY_HIGH:
            logWarn("OpenGLHigh", EngineCore::stringify(src_str, ", ", type_str, ", ", severity_str, ", 0x", std::hex, id, ": ", message, '\n'));
            flushLogs();
            break;
    }
}

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Graphics";

    OpenGLContext::OpenGLContext(SDL_Window* w): window(w) {}

    int OpenGLContext::init()
    {
        logInfo(LOGGER_TAG, "Initialise gl3w");
        if (gl3wInit() != GL3W_OK)
        {
            int err = GetLastError();
            logFatal(LOGGER_TAG, "Failed to intitialise gl3w: ", err);
            flushLogs();
            return GraphicsErrors::GRAPHICS_GL3W_FAILED_TO_INIT;
        }
        flushLogs();
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(openGLMessageCallback, nullptr);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);

        return GraphicsErrors::GRAPHICS_OK;
    }

    void OpenGLContext::swapBuffers()
    {
        SDL_GL_SwapWindow(window);
        logInfo(LOGGER_TAG, "Swapped buffers");
    }
}