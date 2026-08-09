#include "Log.hpp"
#include "Logger2.hpp"



namespace EngineCore
{
    logger Log::instance{};

    void Log::init()
    {

    }

    void Log::setLogLevel(const logger::log_level l)
    {
        instance.set_log_level(l);
    }

    void Log::flush()
    {
        instance.flush_logs();
    }
}