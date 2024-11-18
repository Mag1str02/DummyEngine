#include "DummyEngine/Utils/Debug/Logger.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Application/FileSystem.h"
#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEngine/Utils/Debug/Assert.h"

namespace DE {
    std::string Logger::Record::LogRecordTypeToStr(Type type) {
        switch (type) {
            case Type::Debug: return "Debug";
            case Type::Info: return "Info";
            case Type::Warning: return "Warning";
            case Type::Error: return "Error";
            case Type::Fatal: return "Fatal";
            default: return "None";
        }
    }

    std::string Logger::Record::ToString() const {
        auto type_str   = std::format("[{}]", LogRecordTypeToStr(type));
        auto author_str = author.empty() ? author : std::format("[{}]", author);
        return std::format("[{}] {: >9} {: <20} {}\n", timestamp, type_str, author_str, message);
    }

    SINGLETON_BASE(Logger);
    S_INITIALIZE() {
        FileSystem::CreateDirectory(Config::GetPath(DE_CFG_LOG_PATH));
        auto& log  = m_Streams.emplace(kDefaultLog, LogStream()).first->second;
        auto  path = Config::GetPath(DE_CFG_LOG_PATH) / kDefaultLogFileName;
        log.stream.open(path);
        DE_ASSERT(log.stream.is_open(), "Failed to open default log: {}", path.string());
        return Unit();
    }
    S_TERMINATE() {
        for (auto& [name, stream] : m_Streams) {
            stream.stream.close();
        }
        m_Streams.clear();
        return Unit();
    }

    S_METHOD_IMPL(bool, Open, (const std::string& log_name), (log_name)) {
        auto [it, emplaced] = m_Streams.emplace(log_name, LogStream());
        if (!emplaced) {
            return false;
        }
        auto& log = it->second;
        FileSystem::CreateDirectory(Config::GetPath(DE_CFG_LOG_PATH));
        log.stream.open(Config::GetPath(DE_CFG_LOG_PATH) / (log_name + ".txt"));
        if (!log.stream.is_open()) {
            m_Streams.erase(it);
            return false;
        }
        return true;
    }
    S_METHOD_IMPL(Unit, Close, (const std::string& log_name), (log_name)) {
        m_Streams.erase(log_name);
        return Unit();
    }

    S_METHOD_IMPL(Unit, Log, (const std::string& log, Record&& record), (log, std::move(record))) {
        auto it = m_Streams.find(log);
        if (it == m_Streams.end()) {
            return Unit();
        }
        auto& stream = it->second;

        stream.records.push_back(std::move(record));
        if (stream.records.size() > stream.depth) {
            stream.records.pop_front();
        }
        auto log_string = stream.records.back().ToString();
        Console::PushLog(log_string);
        std::cout << log_string;
        stream.stream << log_string;
        stream.stream.flush();
        if (stream.records.back().type == Record::Type::Fatal) {
            ITerminate();
        }
        return Unit();
    }
    S_METHOD_IMPL(const std::deque<Logger::Record>&, GetRecords, (const std::string& log), (log)) {
        if (!m_Streams.contains(log)) {
            return m_Empty;
        }
        return m_Streams[log].records;
    }
    S_METHOD_IMPL(Unit, SetLogDepth, (U32 depth, const std::string& log), (depth, log)) {
        if (!m_Streams.contains(log)) {
            return Unit();
        }
        m_Streams[log].depth = depth;
        return Unit();
    }

    void LogWithAuthor(const std::string& log, Logger::Record::Type type, const std::string& author, std::string&& message) {
        Logger::Record record;
        record.author    = author;
        record.timestamp = Logger::Record::Clock::now();
        record.type      = type;
        record.message   = std::move(message);

        Logger::Log(log, std::move(record));
    }

    void Log(const std::string& log, Logger::Record::Type type, std::string&& message) {
        LogWithAuthor(log, type, Logger::kDefaultAuthor, std::move(message));
    }

}  // namespace DE