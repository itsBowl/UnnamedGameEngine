#include "App.hpp"
#include "Core/Locator.hpp"
#include <tracy/Tracy.hpp>
#include "GraphicsFactory.hpp"
#include "Logging/Log.hpp"
#include "Graphics/GraphicsAPI.hpp"
#include "SDL3/SDL_properties.h"


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
        Locator::provide(&assetManager);
        int errValue = window.init();
        if (errValue != CoreErrors::CORE_OK)
        {
            Log::fatal(Log::Core, "Failed to initialise window: ", errValue);
            Log::flush();
            return;
        }
        SDL_Window* sdlWindow = window.getWindow();
        HWND hwnd = static_cast<HWND>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(sdlWindow),
            SDL_PROP_WINDOW_WIN32_HWND_POINTER,
            nullptr
        ));
        WindowHandle handle = {};
        if (getActiveGraphicsAPI() == GraphicsAPI::DirectX12)
        {
            handle.handle = hwnd;
        }
        else if (getActiveGraphicsAPI() == GraphicsAPI::OpenGL)
        {
            handle.handle = sdlWindow;
        }
        
        SDL_GetWindowSize(window.getWindow(), &handle.width, &handle.height);
        inputHandler.init();
        render->init(handle);
        PipelineState beginState;
        render->setClearColour(glm::vec4(0.2f, .02f, 0.2f, 1.f));
        render->setPipelineState(beginState);
        running = true;
        IShader* ret = assetManager.shader().load("../Assets/Shaders/Basic").get();
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
        

        Log::flush();
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

        std::vector<std::shared_ptr<Mesh>> testModel = assetManager.mesh().load("../Assets/Models/TestRevoker/model.fbx");
        Log::info(Log::Core, "Model VAO: ", testModel.at(0)->getVAO(), " Model IdxCount: ", testModel.at(0)->getIndexCount());

#pragma region testCube
        //test cube
        std::vector<Vertex> vertices = 
        {
            // Front face (+Z)
            { {-1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { { 1.0f, -1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { { 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { {-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },

            // Back face (-Z)
            { { 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { {-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { {-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { { 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },

            // Left face (-X)
            { {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { {-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { {-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },

            // Right face (+X)
            { { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { { 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { { 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },

            // Top face (+Y)
            { {-1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { { 1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { { 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },

            // Bottom face (-Y)
            { {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { { 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { { 1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { {-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },
        };

        std::vector<uint32_t> indices = 
        {
            // Front
            0, 1, 2,  2, 3, 0,
            // Back
            4, 5, 6,  6, 7, 4,
            // Left
            8, 9, 10, 10, 11, 8,
            // Right
            12, 13, 14, 14, 15, 12,
            // Top
            16, 17, 18, 18, 19, 16,
            // Bottom
            20, 21, 22, 22, 23, 20,
        };
        squareMesh.create(vertices, indices);
#pragma endregion
        

        assetManager.shader().debugPrintShaders();
        int value = 1;
        InputListener testListener = InputListener(&inputHandler, (EngineCore::ListenerID)inputHandler.onKeyPressed([&value](const KeyEvent& e)
            {
                if (e.scancode == SDL_SCANCODE_1)
                {
                    value += 1;
                    if (value > 4) value = 1;
                    Log::info(Log::Core, "Updated Value: ", value);
                }
            }
        ));

        std::shared_ptr<IUniformBuffer> testUBO = GraphicsFactory::createUniformBuffer(sizeof(value));
        testUBO->setData(&value, sizeof(value));


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
            testUBO->setData(&value, sizeof(value));
            //assetManager.shader().get("Basic")->setInt("test", value);
            //render->draw(squareMesh);
            render->draw(testModel, assetManager.shader().get("Basic"), {camera.getUBO(), testUBO});
            render->endFrame();
            if (getActiveGraphicsAPI() == GraphicsAPI::OpenGL)
            {
                window.swapBuffers();
            }
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