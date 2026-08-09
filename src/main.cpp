#include "PCH.hpp"
#include <tracy/tracy.hpp>
#include "Core/App.hpp"
#include "Logging/Logger.hpp"
#include "Logging/Logger2.hpp"
#include "Threading/threadpool.hpp"
#include "confirmACSII.hpp"

static EngineCore::logger logger;


int main(int argc, char** argv)
{
    
    EngineCore::threadpool threads(4);
    logger.set_log_level(EngineCore::logger::log_level::ALL);
    std::cout << "\x1b[44m \x1b[37m" << confirm << "\x1b[0m" << std::endl;
    EngineCore::App* app = new EngineCore::App();
    app->run();
    delete app;

    return 0;
}