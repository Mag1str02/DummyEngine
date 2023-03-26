#pragma once

#include "DummyEngine/Utils/STDIncludes.h"
#include "DummyEngine/Utils/Types.h"
#include "DummyEngine/Utils/Singleton.h"

namespace DE
{
    enum class LogMessageType
    {
        None = 0,
        Info,
        Warning,
        Stage,
        Error,
        Fatal
    };

    std::string LogMessageTypeToStr(LogMessageType type);

    // TODO: Level hierarchy rules. Currently all lvl 0.

    struct LogRecord
    {
        uint32_t       level = 0;
        time_t         time;
        LogMessageType type;
        std::string    author;
        std::string    message;

        std::string ToString() const;
    };

    class Logger : public Singleton<Logger>
    {
        SINGLETON(Logger)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(bool, Open, (const std::string& log_name));
        S_METHOD_DEF(Unit, Close, (const std::string& log_name));

        S_METHOD_DEF(
            Unit,
            Log,
            (LogMessageType type, const std::string& str, const std::string& author = "Anonymous", uint32_t lvl = 0, const std::string& to = ""));
        S_METHOD_DEF(const std::deque<LogRecord>&, GetLog, (const std::string& log = ""));
        S_METHOD_DEF(Unit, SetDepth, (uint32_t depth, const std::string& log = ""));

    private:
        Logger()  = default;
        ~Logger() = default;

        struct LogStream
        {
            std::deque<LogRecord> records;
            std::ofstream         stream;
            uint32_t              depth = 32;
        };
        std::deque<LogRecord>                      m_Empty;
        std::unordered_map<std::string, LogStream> m_Streams;
    };
}  // namespace DE

#if DE_ENABLE_LOGGING == 1
#define LOG_ERROR(...) Logger::Log(LogMessageType::Error, __VA_ARGS__)
#define LOG_INFO(...) Logger::Log(LogMessageType::Info, __VA_ARGS__)
#define LOG_WARNING(...) Logger::Log(LogMessageType::Warning, __VA_ARGS__)
#define LOG_STAGE(...) Logger::Log(LogMessageType::Stage, __VA_ARGS__)
#define LOG_FATAL(...) Logger::Log(LogMessageType::Fatal, __VA_ARGS__)
#else
#define LOG_ERROR(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_STAGE(...)
#define LOG_FATAL(...)
#endif