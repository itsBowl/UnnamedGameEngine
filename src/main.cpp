#include "PCH.hpp"
#include <tracy/tracy.hpp>
#include "Core/App.hpp"
#include "Logging/Logger.hpp"
#include "Logging/Logger2.hpp"
#include "Threading/threading.hpp"
#include "confirmACSII.hpp"




int main(int argc, char** argv)
{
    
    EngineCore::createThreadpool();

    setLogLevel(LogLevel::ALL);
    std::cout << "\x1b[44m \x1b[37m" << confirm << "\x1b[0m" << std::endl;
    EngineCore::App* app = new EngineCore::App();
    app->run();

    EngineCore::releaseThreadpool();

    delete app;
    
    return 0;
}