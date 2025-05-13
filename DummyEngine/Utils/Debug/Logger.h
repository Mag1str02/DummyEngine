#pragma once

#include "DummyEngine/Utils/Helpers/STDAdapters.h"  // IWYU pragma: export
#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/PCH/DC.h"
#include "DummyEngine/Utils/Types/Types.h"

#include <deque>
#include <fstream>
#include <unordered_map>

namespace DummyEngine {

    class Logger : public Singleton<Logger> {
    public:
        enum class RecordType {
            None = 0,
            Debug,
            Info,
            Warning,
            Error,
            Fatal,
        };

        struct Record {
        public:
            using Clock     = std::chrono::system_clock;
            using TimeStamp = std::chrono::time_point<Clock>;

        public:
            static std::string LogRecordTypeToStr(RecordType type);
            std::string        ToString() const;

        public:
            TimeStamp   Timestamp;
            RecordType  Type;
            std::string Author;
            std::string Message;
        };

    public:
        static constexpr const char* kDefaultLog         = "";
        static constexpr const char* kDefaultAuthor      = "";
        static constexpr const char* kDefaultLogFileName = "Log.log";

        SINGLETON(Logger)

        S_METHOD_DEF(bool, Open, (const std::string& log_name));
        S_METHOD_DEF(Unit, Close, (const std::string& log_name));

        S_METHOD_DEF(Unit, Log, (const std::string& log, Record&& record));
        S_METHOD_DEF(Unit, SetLogDepth, (U32 depth, const std::string& log = kDefaultLog));

        S_METHOD_DEF(const std::deque<Record>&, GetRecords, (const std::string& log = kDefaultLog));

    private:
        struct LogStream {
            std::deque<Record> Records;
            std::ofstream      Stream;
            U32                Depth = 32;
        };

        FMutex                                     logs_mutex_;
        std::deque<Record>                         empty_;
        std::unordered_map<std::string, LogStream> streams_;
    };

    void LogWithAuthor(const std::string& log, Logger::RecordType type, const std::string& author, std::string&& message);
    void Log(const std::string& log, Logger::RecordType type, std::string&& message);

}  // namespace DummyEngine

#if DE_ENABLE_LOGGING == 1
#define LOG_AUTHOR(author)                                                                    \
private:                                                                                      \
    static void Log(const std::string& log, Logger::RecordType type, std::string&& message) { \
        LogWithAuthor(log, type, #author, std::move(message));                                \
    }
#define LOG_DEBUG(...) Log(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Debug, std::format(__VA_ARGS__))
#define LOG_INFO(...) Log(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Info, std::format(__VA_ARGS__))
#define LOG_WARNING(...) Log(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Warning, std::format(__VA_ARGS__))
#define LOG_ERROR(...) Log(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Error, std::format(__VA_ARGS__))
#define LOG_FATAL(...) Log(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Fatal, std::format(__VA_ARGS__))

#define LOG_DEBUG_AS(author, ...) \
    LogWithAuthor(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Debug, author, std::format(__VA_ARGS__))
#define LOG_INFO_AS(author, ...) \
    LogWithAuthor(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Info, author, std::format(__VA_ARGS__))
#define LOG_WARNING_AS(author, ...) \
    LogWithAuthor(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Warning, author, std::format(__VA_ARGS__))
#define LOG_ERROR_AS(author, ...) \
    LogWithAuthor(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Error, author, std::format(__VA_ARGS__))
#define LOG_FATAL_AS(author, ...) \
    LogWithAuthor(::DummyEngine::Logger::kDefaultLog, ::DummyEngine::Logger::RecordType::Fatal, author, std::format(__VA_ARGS__))
#else
#define LOG_AUTHOR(author)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#define LOG_DEBUG_AS(author, ...)
#define LOG_INFO_AS(author, ...)
#define LOG_WARNING_AS(author, ...)
#define LOG_ERROR_AS(author, ...)
#define LOG_FATAL_AS(author, ...)
#endif