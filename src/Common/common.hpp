#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include "sized_numerics.hpp"

namespace EngineCore
{
    std::string stringify_utf32(const u32 codepoint);

    [[nodiscard]] std::string stringify_duration(const std::chrono::microseconds duration);
    [[nodiscard]] std::string stringify_secs(const f64 seconds);
    [[nodiscard]] std::string stringify_millis(const f64 millis);
    [[nodiscard]] std::string stringify_bytes(const unsigned long bytes);
    [[nodiscard]] std::string stringify_count(const unsigned long count_num, std::string_view count_of);
    [[nodiscard]] std::string stringify_char(const char c);

    [[nodiscard]] constexpr std::string stringify(const auto&... values)
    {
        std::stringstream format_stream;
        format_stream << std::fixed << std::setprecision(5);
        (format_stream << ... << values);
        return format_stream.str();
    }
}