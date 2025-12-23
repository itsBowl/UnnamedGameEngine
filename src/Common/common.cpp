
#include "PCH.hpp"
#include "common.hpp"

#include <algorithm>
#include <bit>
#include "tracy/Tracy.hpp"

namespace EngineCore
{
    void string_replace(std::string& str, const char from, const char to)
    {
        for (char& idx : str)
        {
            if (idx == from) idx = to;
        }
    }

    std::string stringify_utf32(const unsigned int codepoint)
    {
        std::string result;
        if (codepoint < 0x80)
        {
            result += static_cast<char>(codepoint);
        }
        else if (codepoint < 0x800)
        {
            result += static_cast<char>((codepoint >> 6) | 0xC0);
            result += static_cast<char>((codepoint & 0x3F) | 0x80);
        }
        else if (codepoint < 0x10000)
        {
            result += static_cast<char>((codepoint >> 12) | 0xE0);
            result += static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80);
            result += static_cast<char>((codepoint & 0x3F) | 0x80);
        }
        else if (codepoint < 0x110000)
        {
            result += static_cast<char>((codepoint >> 18) | 0xF0);
            result += static_cast<char>(((codepoint >> 12) & 0x3F) | 0x80);
            result += static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80);
            result += static_cast<char>((codepoint & 0x3F) | 0x80);
        }

        return result;
    }

    std::string stringify_duration(const std::chrono::microseconds duration)
    {
        ZoneScoped;

        const std::chrono::microseconds us = duration;
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        const auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);

        std::stringstream format_stream;
        format_stream << std::fixed << std::setprecision(2);

        if (us.count() < 1000)
        {
            return std::to_string(us.count()) + "us";
        }

        if (ms.count() < 1000)
        {
            format_stream << static_cast<f64>(ms.count()) + static_cast<f64>(us.count() % 1000) / 1000.0;
            const std::string ms_fractional = format_stream.str();
            return ms_fractional + "ms";
        }

        format_stream << static_cast<f64>(s.count()) + static_cast<f64>(ms.count() % 1000) / 1000.0;
        const std::string ms_fractional = format_stream.str();
        return ms_fractional + "s";
    }

    std::string stringify_secs(const f64 seconds)
    {
        return stringify_millis(seconds * 1000);
    }

    std::string stringify_millis(const f64 millis)
    {
        return stringify_duration(std::chrono::microseconds {static_cast<long>(millis * 1000)});
    }

    std::string stringify_bytes(const unsigned long bytes)
    {
        ZoneScoped;
        std::stringstream result;
        result << std::fixed << std::setprecision(2);
        const f64 bytes_fractional = bytes;
        f64 bytes_scaled;
        if (bytes < 1e3) bytes_scaled = bytes;
        else if (bytes < 1e6) bytes_scaled = bytes_fractional / 1e3;
        else if (bytes < 1e9) bytes_scaled = bytes_fractional / 1e6;
        else bytes_scaled = bytes_fractional / 1e9;
        result << bytes_scaled;

        if (bytes < 1e3) result << " bytes";
        else if (bytes < 1e6) result << " kb";
        else if (bytes < 1e9) result << " mb";
        else result << " gb";

        return result.str();
    }

    std::string stringify_count(const unsigned long count_num, const std::string_view count_of)
    {
        std::stringstream result;
        result << count_num << " " << count_of;
        if (count_num != 1) result << "s";
        return result.str();
    }

    std::string stringify_char(const char c)
    {
        return std::string(1, c);
    }
}