#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Helpers/StringOperations.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {
    enum class LogMessageType { None = 0, Debug, Info, Warning, Error, Fatal };

    std::string LogMessageTypeToStr(LogMessageType type);

    struct LogRecord {
        time_t         time;
        LogMessageType type;
        std::string    author;
        std::string    message;

        std::string ToString() const;
    };

    class Logger : public Singleton<Logger> {
        SINGLETON(Logger)
    public:
        S_METHOD_DEF(bool, Open, (const std::string& log_name));
        S_METHOD_DEF(Unit, Close, (const std::string& log_name));

        template <typename... Message> static void Log(LogMessageType type, const std::string& author, Message... message) {
            Get().LogInternal(type, author, "", StrCat(message...));
        }
        template <typename... Message> static void LogTo(LogMessageType type, const std::string& author, const std::string& to, Message... message) {
            Get().LogInternal(type, author, to, StrCat(message...));
        }
        S_METHOD_DEF(const std::deque<LogRecord>&, GetLog, (const std::string& log = ""));
        S_METHOD_DEF(Unit, SetDepth, (U32 depth, const std::string& log = ""));

    private:
        std::mutex m_Mutex;
        void LogInternal(LogMessageType type, const std::string& author, const std::string& to, const std::string& str);

        struct LogStream {
            std::deque<LogRecord> records;
            std::ofstream         stream;
            U32                   depth = 32;
        };
        std::deque<LogRecord>                      m_Empty;
        std::unordered_map<std::string, LogStream> m_Streams;
    };
}  // namespace DE

#if DE_ENABLE_LOGGING == 1
#define LOG_DEBUG(...) Logger::Log(LogMessageType::Debug, __VA_ARGS__)
#define LOG_INFO(...) Logger::Log(LogMessageType::Info, __VA_ARGS__)
#define LOG_WARNING(...) Logger::Log(LogMessageType::Warning, __VA_ARGS__)
#define LOG_ERROR(...) Logger::Log(LogMessageType::Error, __VA_ARGS__)
#define LOG_FATAL(...) Logger::Log(LogMessageType::Fatal, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#endif