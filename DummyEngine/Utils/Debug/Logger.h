#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {

    class Logger : public Singleton<Logger> {
    public:
        struct Record {
        public:
            enum class Type {
                None = 0,
                Debug,
                Info,
                Warning,
                Error,
                Fatal,
            };
            using Clock     = std::chrono::system_clock;
            using TimeStamp = std::chrono::time_point<Clock>;

        public:
            static std::string LogRecordTypeToStr(Type type);
            std::string        ToString() const;

        public:
            TimeStamp   timestamp;
            Type        type;
            std::string author;
            std::string message;
        };

    public:
        static constexpr std::string kDefaultLog         = "";
        static constexpr std::string kDefaultAuthor      = "";
        static constexpr std::string kDefaultLogFileName = "Log.log";

        SINGLETON(Logger)

        S_METHOD_DEF(bool, Open, (const std::string& log_name));
        S_METHOD_DEF(Unit, Close, (const std::string& log_name));

        S_METHOD_DEF(Unit, Log, (const std::string& log, Record&& record));
        S_METHOD_DEF(Unit, SetLogDepth, (U32 depth, const std::string& log = kDefaultLog));

        S_METHOD_DEF(const std::deque<Record>&, GetRecords, (const std::string& log = kDefaultLog));

    private:
        struct LogStream {
            std::deque<Record> records;
            std::ofstream      stream;
            U32                depth = 32;
        };
        std::deque<Record>                         m_Empty;
        std::unordered_map<std::string, LogStream> m_Streams;
    };

    void LogWithAuthor(const std::string& log, Logger::Record::Type type, const std::string& author, std::string&& message);
    void Log(const std::string& log, Logger::Record::Type type, std::string&& message);

}  // namespace DE

#if DE_ENABLE_LOGGING == 1
#define LOGGER_AUTHOR(author)                                                                   \
private:                                                                                        \
    static void Log(const std::string& log, Logger::Record::Type type, std::string&& message) { \
        LogWithAuthor(log, type, #author, std::move(message));                                  \
    }
#define LOG_DEBUG(...) Log(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Debug, std::format(__VA_ARGS__))
#define LOG_INFO(...) Log(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Info, std::format(__VA_ARGS__))
#define LOG_WARNING(...) Log(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Warning, std::format(__VA_ARGS__))
#define LOG_ERROR(...) Log(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Error, std::format(__VA_ARGS__))
#define LOG_FATAL(...) Log(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Fatal, std::format(__VA_ARGS__))
#define LOG_DEBUG_AS(author, ...) LogWithAuthor(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Debug, author, std::format(__VA_ARGS__))
#define LOG_INFO_AS(author, ...) LogWithAuthor(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Info, author, std::format(__VA_ARGS__))
#define LOG_WARNING_AS(author, ...) LogWithAuthor(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Warning, author, std::format(__VA_ARGS__))
#define LOG_ERROR_AS(author, ...) LogWithAuthor(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Error, author, std::format(__VA_ARGS__))
#define LOG_FATAL_AS(author, ...) LogWithAuthor(::DE::Logger::kDefaultLog, ::DE::Logger::Record::Type::Fatal, author, std::format(__VA_ARGS__))
#else
#define LOGGER_AUTHOR(author)
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