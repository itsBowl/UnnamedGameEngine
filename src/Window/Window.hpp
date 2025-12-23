#pragma once
#include "SDL3/SDL.h"
#include "Graphics/GraphicsContext.hpp"

#include "../Logging/Logger2.hpp"

namespace EngineCore
{
    class Window
    {
    public:

        SDL_Window* operator()()
        {
            return window;
        }
        int init();
        int initGraphics();

        SDL_Window* getWindow() {return window;}
        
        int update();
        
    private:
        SDL_Window* window;
        GraphicsContext* context;
        

    };
}