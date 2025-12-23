#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace EngineCore 
{

    class Logger
    {
        public:
            static void init();
            inline static std::shared_ptr<spdlog::logger>& getLogger() {return logger;}
        private:
            static std::shared_ptr<spdlog::logger> logger;
    };
}

#define LOG_TRACE(...) :: EngineCore::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  :: EngineCore::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  :: EngineCore::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) :: EngineCore::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) :: EngineCore::Logger::getLogger()->critical(__VA_ARGS__)