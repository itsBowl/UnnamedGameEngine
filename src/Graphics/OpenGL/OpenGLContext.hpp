#pragma once

#include "../GraphicsContext.hpp"

#include "GL/gl3w.h"
#include "SDL3/SDL.h"




namespace EngineCore
{
    struct WindowHandle;
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext();
        OpenGLContext(SDL_Window*);

        int init(const WindowHandle& wh);
        virtual int init() override;
        virtual void swapBuffers() override;

    private:
        SDL_Window* window;
    };
}

