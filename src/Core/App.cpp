#include "PCH.hpp"
#include "App.hpp"
#include "Graphics/OpenGLContext.hpp"
#include <tracy/Tracy.hpp>

namespace EngineCore
{
    App::App()
    {

    }

    void App::run()
    {
        window.init();
        bool run = true;
        while (run)
        {
            if (window.update() == CoreEvents::QUIT)
                run = false;
            flushLogs();
            FrameMark;
        }
    }

}