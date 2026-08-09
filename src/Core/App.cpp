#include "App.hpp"
#include "Core/Locator.hpp"
#include "../Graphics/OpenGL/OpenGLContext.hpp"
#include <tracy/Tracy.hpp>
#include "GraphicsFactory.hpp"
#include "Logging/Log.hpp"


namespace EngineCore
{
    App::App() : running(false)
    {

    }

    void App::init()
    {
        Locator::provide(&updateSystem);
        Locator::provide(&inputHandler);
        render = GraphicsFactory::createRender();
        Locator::provide(render.get());
        Locator::provide(&time);
        int errValue = window.init();
        if (errValue != CoreErrors::CORE_OK)
        {
            Log::fatal(Log::Core, "Failed to initialise window: ", errValue);
            Log::flush();
            return;
        }

        inputHandler.init();
        render->init();
        render->setClearColour(glm::vec4(0.2f, .02f, 0.2f, 1.f));
        render->setDepthTest(true);
        running = true;
        auto ret = shaderLib.load("../Assets/Shaders/Basic");
        if (ret == nullptr)
        {
            Log::error(Log::Core, "Basic Shader Failed");
            Log::flush();
            onWindowClose();
        }
        

        inputHandler.onKeyPressed([this](const KeyEvent& e)
        {
           onKeyPressed(e); 
        });

        inputHandler.onMouseMoved([this](const MouseMoveEvent& e)
        {
            onMouseMoved(e);
        });
        

        
    }

    void App::run()
    {
        init();
        if (running == false)
        {
            shutdown();
            return;
        }
        Camera camera(inputHandler, window);

        //test tri setup
        std::vector<Vertex> vertices = {
        { {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { { 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        };
        std::vector<uint32_t> indices = { 0, 1, 2 };
        triangleMesh.create(vertices, indices);

        shaderLib.debugPrintShaders();
        int value = 1;
        InputListener testListener = InputListener(&inputHandler, (EngineCore::ListenerID)inputHandler.onKeyPressed([&value](const KeyEvent& e)
            {
                if (e.scancode == SDL_SCANCODE_1)
                {
                    value += 1;
                    if (value > 3) value = 1;
                    Log::info(Log::Core, "Updated Value: ", value);
                }
            }
        ));


        while (running)
        {
            time.tick();
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

            updateSystem.updateAll(time.getDeltaTime());
            
            //render loop
            render->beginFrame();
            render->clear();
            std::shared_ptr<IShader> shader = shaderLib.get("Basic");
            shader->bind();
            shader->setInt("test", value);
            render->draw(triangleMesh);
            shader->unbind();
            render->endFrame();
            window.swapBuffers();
            Log::flush();
            FrameMark;
        }

        shutdown();
    }

    void App::shutdown()
    {
        Log::info(Log::Core, "Shut down");
        
        Log::flush();
    }

    void App::onWindowClose()
    {
        Log::info(Log::Core, "Window closed");
        running = false;
    }

    void App::onWindowResize(int w, int h)
    {
        Log::info(Log::Core, "Window resized: ", w, " ", h);
        render->setViewport(0, 0, w, h);
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
        //Log::info("ENGINE_CORE", "Mouse moved: ", e.dx, ", ", e.dy);
    }

}