#pragma once

#include "../GraphicsContext.hpp"

#include "GL/gl3w.h"
#include "SDL3/SDL.h"


namespace EngineCore
{

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(SDL_Window*);

        virtual int init() override;
        virtual void swapBuffers() override;

    private:
        SDL_Window* window;
    };
}

