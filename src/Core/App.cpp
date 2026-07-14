#include "App.hpp"
#include "../Graphics/OpenGL/OpenGLContext.hpp"
#include <tracy/Tracy.hpp>


namespace EngineCore
{
    App::App() : running(false)
    {

    }

    void App::init()
    {
        int errValue = window.init();
        if (errValue != CoreErrors::CORE_OK)
        {
            logFatal("ENGINE_CORE", "Failed to initialise window: ", errValue);
            flushLogs();
            return;
        }

        inputHandler.init();
        render.init();
        render.setClearColour(glm::vec4(0.2f, .02f, 0.2f, 1.f));
        render.setDepthTest(true);
        shaderLib.load("../Assets/Shaders/Basic");
        

        inputHandler.onKeyPressed([this](const KeyEvent& e)
        {
           onKeyPressed(e); 
        });

        inputHandler.onMouseMoved([this](const MouseMoveEvent& e)
        {
            onMouseMoved(e);
        });
        

        running = true;
    }

    void App::run()
    {
        init();
        render3d.setupTri();
        shaderLib.debugPrintShaders();
        int value = 1;
        InputListener testListnere = InputListener(&inputHandler, (EngineCore::ListenerID)inputHandler.onKeyPressed([&](const KeyEvent& e)
            {
                if (e.scancode == SDL_SCANCODE_1)
                {
                    value += 1;
                    if (value > 3) value = 1;
                }
            }
        ));


        while (running)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)
                {
                    onWindowClose();
                    break;
                }

                if (e.type == SDL_EVENT_WINDOW_RESIZED)
                {
                    onWindowResize(e.window.data1, e.window.data2);
                }
                inputHandler.processEvent(e);
            }
            
            //render loop
            render.beginFrame();
            render.clear();
            auto shader = shaderLib.get("Basic").get();
            shader->bind();
            shader->setInt("test", value);
            render.drawIndexed(render3d.getTri(), render3d.getTri().getIndexCount());
            shader->unbind();
            render.endFrame();
            window.swapBuffers();
            flushLogs();
            FrameMark;
        }
    }

    void App::shutdown()
    {
        logInfo("ENGINE_CORE", "Shut down");
        flushLogs();
    }

    void App::onWindowClose()
    {
        logInfo("ENGINE_CORE", "Window closed");
        running = false;
    }

    void App::onWindowResize(int w, int h)
    {
        logInfo("ENGINE_CORE", "Window resized: ", w, " ", h);
        render.setViewport(0, 0, w, h);
    }

    void App::onKeyPressed(const KeyEvent& e)
    {
        //tmp
        if (e.scancode == SDL_SCANCODE_ESCAPE)
        {
            onWindowClose();
        }
    }

    void App::onMouseMoved(const MouseMoveEvent& e)
    {
        //tmp
        //logInfo("ENGINE_CORE", "Mouse moved: ", e.dx, ", ", e.dy);
    }

}