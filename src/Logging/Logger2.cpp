#include "PCH.hpp"
#include "logger2.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <tracy/Tracy.hpp>

#include "../Threading/threading.hpp"
#include "../Common/common.hpp"



    class LoggingFlushTask final : public EngineCore::ThreadpoolTask
    {
        public:
        std::vector<std::string> data;

        void execute() override
        {
            ZoneScopedN("Flush logs task");
            constexpr std::string_view timeFormat = std::string_view("\u001B[36m\u001B[1m");
            const std::time_t currentTime = std::time(nullptr);

            const auto& now = std::localtime(&currentTime);

            const std::string timeStamp = EngineCore::stringify('[', timeFormat, now->tm_hour, ":", now->tm_min, ":", now->tm_sec, AnsiCodes::ANSI_RESET, "]");
            
            for (const std::string& msg : data)
            {
                if (msg != AnsiCodes::ANSI_DELETE_LINE) std::cout << timeStamp;
                std::cout << msg;
            }
        
        }
    };

    static std::mutex loggingLock;
    static LogLevel loggingLevel = LogLevel::NORMAL;
    static int32_t logRepeatCount = 0;
    static std::string lastMsg;
    static std::string lastTag;
    static std::stringstream builder;
    static std::vector<std::string> messageQueue;
    static bool hasBufferedMsgs = false;

    static LoggingFlushTask* currentFlushTask = nullptr;

    void setLogLevel(const LogLevel l)
    {
        loggingLock.lock();
        loggingLevel = l;
        loggingLock.unlock();
    }

    void flushLogs()
    {
        ZoneScoped;
        loggingLock.lock();

        if (!hasBufferedMsgs)
        {
            loggingLock.unlock();
            return;
        }

        if (currentFlushTask != nullptr && !currentFlushTask->completed())
        {
            loggingLock.unlock();
            return;
        }

        delete currentFlushTask;
        currentFlushTask = nullptr;

        currentFlushTask = new LoggingFlushTask();
        currentFlushTask->data = messageQueue;
        currentFlushTask->enqueue();

        messageQueue.clear();
        hasBufferedMsgs = false;

        loggingLock.unlock();
    }

    inline void writeTag(const std::string_view fmt, const std::string_view colour, const std::string_view tag)
    {
        builder << '[' << fmt << colour << tag << AnsiCodes::ANSI_RESET << ']';
    }

    inline void writeRepeatTag(const std::string_view colour)
    {
        builder << AnsiCodes::ANSI_RESET << " [" << AnsiCodes::ANSI_BOLD << colour << "+" << std::to_string(logRepeatCount) << AnsiCodes::ANSI_RESET << ']';
    }

    void log(const std::string_view tag, const std::string_view msg, const std::string_view type, const std::string_view colour)
    {
        ZoneScoped;
        
        loggingLock.lock();

        const bool isRepeat = msg == lastMsg && tag == lastTag;

        if (isRepeat)
        {
            logRepeatCount++;
            messageQueue.emplace_back(AnsiCodes::ANSI_DELETE_LINE);
        }
        else
        {
            logRepeatCount = 0;
            lastMsg = msg;
            lastTag = tag;
        }

        builder << EngineCore::stringify("[", AnsiCodes::ANSI_BOLD, AnsiCodes::ANSI_BRIGHT_BLUE, "T", std::setfill('0'), std::setw(3), EngineCore::getCurrentThreadID(), AnsiCodes::ANSI_RESET, ']');


        if (!type.empty()) writeTag(AnsiCodes::ANSI_BOLD, colour, type);
        if (!tag.empty()) writeTag(AnsiCodes::ANSI_RESET, AnsiCodes::ANSI_PURPLE, tag);

        builder << ' ' << colour << msg << AnsiCodes::ANSI_RESET;

        if (isRepeat) writeRepeatTag(colour);
        
        builder << std::endl;

        messageQueue.push_back(builder.str());
        builder.str("");

        hasBufferedMsgs = true;

        loggingLock.unlock();
    }

    void logDebug(const std::string_view tag, const std::string_view msg)
    {
        if (loggingLevel > LogLevel::NORMAL) log(tag, msg, "DEBUG", AnsiCodes::ANSI_GREEN);
    }

    void logInfo(const std::string_view tag, const std::string_view msg)
    {
        if (loggingLevel > LogLevel::REDUCED) log(tag, msg, "INFO", AnsiCodes::ANSI_BLUE);
    }

    void logPerf(const std::string_view tag, const std::string_view msg)
    {
        if (loggingLevel > LogLevel::REDUCED) log(tag, msg, "PERF", AnsiCodes::ANSI_CYAN);
    }

    void logWarn(const std::string_view tag, const std::string_view msg)
    {
        if (loggingLevel > LogLevel::NONE) log(tag, msg, "WARN", AnsiCodes::ANSI_BRIGHT_YELLOW);
    }

    void logError(const std::string_view tag, const std::string_view msg)
    {
        if (loggingLevel > LogLevel::NONE) log(tag, msg, "ERROR", AnsiCodes::ANSI_RED);
    }

    void logFatal(const std::string_view tag, const std::string_view msg)
    {
        constexpr std::string_view fatal = std::string_view("\u001B[91m\u001B[1m\u001B[4m");
        log(tag, msg, "FATAL", fatal);
    }

    void logSecret(const std::string_view tag, const std::string_view msg)
    {
        log(tag, msg, "SECRET", AnsiCodes::ANSI_BRIGHT_YELLOW);
    }