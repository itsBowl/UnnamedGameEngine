#include "PCH.hpp"
#include <tracy/tracy.hpp>
#include "Core/App.hpp"
#include "Logging/Logger.hpp"
#include "Logging/Logger2.hpp"
#include "Threading/threadpool.hpp"
#include "confirmACSII.hpp"
#include "Graphics/GraphicsAPI.hpp"

static EngineCore::logger logger;

EngineCore::GraphicsAPI testAPISwitch()
{
    std::cout << "Select Graphics API:\n1) OpenGL\n2)DirectX12\nVulkan(NOT AVAILABLE)\n> ";

    std::string input;
    std::getline(std::cin, input);
    
    if (input == "1") return EngineCore::GraphicsAPI::OpenGL;
    else if (input == "2") return EngineCore::GraphicsAPI::DirectX12;
    else
    {
        std::cout << "Pick something properly please (:\n";
    }

    return testAPISwitch();

}


int main(int argc, char** argv)
{
    EngineCore::GraphicsAPI api = testAPISwitch();
    EngineCore::setActiveGraphicsAPI(api);

    EngineCore::threadpool threads(4);
    logger.set_log_level(EngineCore::logger::log_level::ALL);
    std::cout << "\x1b[44m \x1b[37m" << confirm << "\x1b[0m" << std::endl;
    EngineCore::App* app = new EngineCore::App();
    app->run();
    delete app;

    return 0;
}