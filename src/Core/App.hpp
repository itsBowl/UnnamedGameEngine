#pragma once
#include "PCH.hpp"
#include "Window/Window.hpp"
#include "Events/Events.hpp"
#include "Errors/Errors.hpp"
#include "Graphics/GraphicsContext.hpp"

int main(int argc, char** argv);

namespace EngineCore
{
    class App
    {
    public:
        App();
        
    private:
        void run();
        bool onWindowClose();
        bool onWindowResize();

        Window window;

        friend int ::main(int argc, char** argv);


    };
    

}