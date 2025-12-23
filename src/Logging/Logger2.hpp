#pragma once
#include <sstream>
#include <string>
#include "../Common/common.hpp"
#include "../Common/sized_numerics.hpp"
#include "glm/glm.hpp"


#define MAKE_PRINTABLE(type, func) inline std::ostream& operator << (std::ostream& lhs, const type& rhs) { \
    lhs << func; \
    return lhs; }

MAKE_PRINTABLE(glm::vec2, '[' << rhs.x << ", " << rhs.y << ']');
MAKE_PRINTABLE(glm::ivec2, '[' << rhs.x << ", " << rhs.y << ']');
MAKE_PRINTABLE(glm::dvec2, '[' << rhs.x << ", " << rhs.y << ']');
MAKE_PRINTABLE(glm::vec3, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ']');
MAKE_PRINTABLE(glm::ivec3, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ']');
MAKE_PRINTABLE(glm::dvec3, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ']');
MAKE_PRINTABLE(glm::vec4, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ']');
MAKE_PRINTABLE(glm::ivec4, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ']');
MAKE_PRINTABLE(glm::dvec4, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ']');

#undef MAKE_PRINTABLE



    enum class LogLevel : uint8_t{
        NONE = 0,
        REDUCED,
        NORMAL,
        ALL
    };

    namespace AnsiCodes
    {
        constexpr std::string_view ANSI_RESET = "\u001B[0m";
        constexpr std::string_view ANSI_BOLD = "\u001B[1m";
        constexpr std::string_view ANSI_UNDERLINE = "\u001B[4m";
        constexpr std::string_view ANSI_BLACK = "\u001B[30m";
        constexpr std::string_view ANSI_RED = "\u001B[31m";
        constexpr std::string_view ANSI_BRIGHT_RED = "\u001B[91m";
        constexpr std::string_view ANSI_GREEN = "\u001B[32m";
        constexpr std::string_view ANSI_BRIGHT_GREEN = "\u001B[92m";
        constexpr std::string_view ANSI_YELLOW = "\u001B[33m";
        constexpr std::string_view ANSI_BRIGHT_YELLOW = "\u001B[93m";
        constexpr std::string_view ANSI_BLUE = "\u001B[34m";
        constexpr std::string_view ANSI_BRIGHT_BLUE = "\u001B[94m";
        constexpr std::string_view ANSI_PURPLE = "\u001B[35m";
        constexpr std::string_view ANSI_BRIGHT_PURPLE = "\u001B[95m";
        constexpr std::string_view ANSI_CYAN = "\u001B[36m";
        constexpr std::string_view ANSI_BRIGHT_CYAN = "\u001B[96m";
        constexpr std::string_view ANSI_GREY = "\u001B[37m";
        constexpr std::string_view ANSI_WHITE = "\u001B[97m";
        constexpr std::string_view ANSI_DELETE_LINE = "\u001B[1A\u001B[2K\r";
    }

    void setLogLevel(const LogLevel);
    void flushLogs();
    void log(const std::string_view, const std::string_view, const std::string_view, const std::string_view);

    void logDebug(const std::string_view, const std::string_view);
    void logInfo(const std::string_view, const std::string_view);
    void logPerf(const std::string_view, const std::string_view);
    void logWarn(const std::string_view, const std::string_view);
    void logError(const std::string_view, const std::string_view);
    void logFatal(const std::string_view, const std::string_view);
    void logSecret(const std::string_view, const std::string_view);

    #define VARARG_DEF(type) void type(const std::string_view id, const std::string_view first, const auto... rest) { type(id, EngineCore::stringify(first, rest...)); }

    VARARG_DEF(logDebug)
    VARARG_DEF(logInfo)
    VARARG_DEF(logPerf)
    VARARG_DEF(logWarn)
    VARARG_DEF(logError)
    VARARG_DEF(logFatal)
    VARARG_DEF(logSecret)

    #undef VARARG_DEF
