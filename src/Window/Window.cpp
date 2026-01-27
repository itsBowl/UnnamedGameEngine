#include "PCH.hpp"
#include "Window.hpp"
#include "../Errors/Errors.hpp"
#include "../Events/Events.hpp"
#include "../Graphics/OpenGL/OpenGLContext.hpp"
#include <tracy/Tracy.hpp>

namespace EngineCore
{
    const std::string LOGGER_TAG = "Window";
    int Window::init()
    {
        logInfo(LOGGER_TAG, "Initialising SDL");
        if (!SDL_Init(SDL_INIT_VIDEO))
            return CoreErrors::SDL_INIT_FAIL;

        window = SDL_CreateWindow("GameEngine",
            640, 480,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

        if (SDL_GL_CreateContext(window) == NULL)
        {
            logFatal(LOGGER_TAG, SDL_GetError());
            return CoreErrors::SDL_FAILED_TO_CREATE_CONTEXT;
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetSwapInterval(0);
        flushLogs();
        if (!initGraphics())
        {
            return CoreErrors::GRAPHICS_FAILED_TO_INIT;
        }

        return CoreErrors::CORE_OK;
    }

    int Window::initGraphics()
    {
        context = new OpenGLContext(window);
        return context->init();
    }

    int Window::update()
    {
        ZoneScoped;
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return CoreEvents::QUIT;
        }
        context->swapBuffers();
    }
}