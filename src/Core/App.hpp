#pragma once
#include "Window/Window.hpp"
#include "Events/Events.hpp"
#include "Errors/Errors.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Render/IRender.hpp"
#include "IO/InputHandler.hpp"
#include "IO/InputListener.hpp"

#include "Shader/ShaderLib.hpp"
#include "UpdateSystem.hpp"
#include "Time.hpp"
#include "Graphics/Camera/Camera.hpp"

#include "Asset/Mesh/Mesh.hpp"

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
        Time time;
        InputHandler inputHandler;
        UpdateSystem updateSystem;
        std::unique_ptr<IRender> render;
        Mesh triangleMesh;
        ShaderLibrary shaderLib;
        bool running = false;

        friend int ::main(int argc, char** argv);


    };
    

}