#include "PCH.hpp"
#include "Window.hpp"
#include "Errors/Errors.hpp"
#include "Events/Events.hpp"
#include "Graphics/OpenGL/OpenGLContext.hpp"
#include "Graphics/GraphicsAPI.hpp"
#include <tracy/Tracy.hpp>

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Window";
    int Window::init()
    {
        Log::info(LOGGER_TAG, "Initialising SDL");
        if (!SDL_Init(SDL_INIT_VIDEO))
            return CoreErrors::SDL_INIT_FAIL;

        uint64_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
        
        if (getActiveGraphicsAPI() == GraphicsAPI::OpenGL)
        {
            flags |= SDL_WINDOW_OPENGL;
        }
        else if (getActiveGraphicsAPI() == GraphicsAPI::Vulkan)
        {
            flags |= SDL_WINDOW_VULKAN;
        }

        window = SDL_CreateWindow("GameEngine",
            640, 480,
            flags
        );
        
        if (getActiveGraphicsAPI() == GraphicsAPI::OpenGL)
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetSwapInterval(0);
            if (SDL_GL_CreateContext(window) == NULL)
            {
                Log::fatal(LOGGER_TAG, "Error creating window: ", SDL_GetError());
                return CoreErrors::SDL_FAILED_TO_CREATE_CONTEXT;
            }
        } 
        Log::flush();
        /*
        if (initGraphics() != GraphicsErrors::GRAPHICS_OK)
        {
            return CoreErrors::GRAPHICS_FAILED_TO_INIT;
        }
        */
        return CoreErrors::CORE_OK;
    }

    int Window::initGraphics()
    {
        context = new OpenGLContext(window);
        return context->init();
    }

    void Window::swapBuffers()
    {
        context->swapBuffers();
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
        return CoreEvents::NONE;
    }
}