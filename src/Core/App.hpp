#pragma once
#include "PCH.hpp"
#include "Window/Window.hpp"
#include "Events/Events.hpp"
#include "Errors/Errors.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/OpenGL/Render/Render.hpp"
#include "IO/InputHandler.hpp"
#include "Render/Render3d/Render3d.hpp"

int main(int argc, char** argv);

namespace EngineCore
{
    class App
    {
    public:
        App();
        
    private:
        void run();
        void init();
        void shutdown();
        void onWindowClose();
        void onWindowResize(int w, int h);
        void onKeyPressed(const KeyEvent& e);
        void onMouseMoved(const MouseMoveEvent& e);

        Window window;
        InputHandler inputHandler;
        Render render;
        Render3d render3d;
        bool running = false;

        friend int ::main(int argc, char** argv);


    };
    

}