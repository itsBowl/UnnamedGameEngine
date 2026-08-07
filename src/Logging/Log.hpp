#pragma once

#include "Logger2.hpp"

namespace EngineCore
{
    class Log
    {
    public:

        static inline const logger::logger_tag Core{"EngineCore", ansi_formatting::red};
        static inline const logger::logger_tag Graphics{"Graphics", ansi_formatting::orange};
        static inline const logger::logger_tag Asset{"Asset", ansi_formatting::bright_blue};
        
        static void init();

        static void setLogLevel(const logger::log_level l);

        static void flush();

        template<typename... Args>
        static void debug(const logger::logger_tag& tag, Args&&... args)
        {
            instance.log_debug(tag, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void debug(const std::string& tag, Args&&... args)
        {
            debug(logger::logger_tag(tag, ""), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void info(const logger::logger_tag& tag, Args&&... args)
        {
            instance.log_info(tag, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void info(const std::string& tag, Args&&... args)
        {
            info(logger::logger_tag(tag, ""), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void perf(const logger::logger_tag& tag, Args&&... args)
        {
            instance.log_performance(tag, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void perf(const std::string& tag, Args&&... args)
        {
            perf(logger::logger_tag(tag, ""), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void warn(const logger::logger_tag& tag, Args&&... args)
        {
            instance.log_warn(tag, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void warn(const std::string& tag, Args&&... args)
        {
            warn(logger::logger_tag(tag, ""), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void error(const logger::logger_tag& tag, Args&&... args)
        {
            instance.log_error(tag, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void error(const std::string& tag, Args&&... args)
        {
            error(logger::logger_tag(tag, ""), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void fatal(const logger::logger_tag& tag, Args&&... args)
        {
            instance.log_fatal(tag, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void fatal(const std::string& tag, Args&&... args)
        {
            fatal(logger::logger_tag(tag, ""), std::forward<Args>(args)...);
        }

    private:
        static logger instance;
    };
}