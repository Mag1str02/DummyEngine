#include "Logger.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Application/FileSystem.h"
#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEngine/Utils/Debug/Assert.h"

#include <chrono>  // IWYU pragma: keep
#include <format>
#include <iostream>

namespace DummyEngine {

    std::string Logger::Record::LogRecordTypeToStr(RecordType type) {
        switch (type) {
            case RecordType::Debug: return "Debug";
            case RecordType::Info: return "Info";
            case RecordType::Warning: return "Warning";
            case RecordType::Error: return "Error";
            case RecordType::Fatal: return "Fatal";
            default: return "None";
        }
    }

    std::string Logger::Record::ToString() const {
        auto type_str   = std::format("[{}]", LogRecordTypeToStr(Type));
        auto author_str = Author.empty() ? Author : std::format("[{}]", Author);
        return std::format("[{}] {: >9} {: <20} {}\n", Timestamp, type_str, author_str, Message);
    }

    SINGLETON_BASE(Logger);
    S_INITIALIZE() {
        FileSystem::CreateDirectory(Config::Get().LogPath);
        auto& log  = streams_.emplace(kDefaultLog, LogStream()).first->second;
        auto  path = Config::Get().LogPath / kDefaultLogFileName;
        log.Stream.open(path);
        DE_ASSERT(log.Stream.is_open(), "Failed to open default log: {}", path.string());
        return Unit();
    }
    S_TERMINATE() {
        for (auto& [name, stream] : streams_) {
            stream.Stream.close();
        }
        streams_.clear();
        return Unit();
    }

    S_METHOD_IMPL(bool, Open, (const std::string& log_name), (log_name)) {
        auto [it, emplaced] = streams_.emplace(log_name, LogStream());
        if (!emplaced) {
            return false;
        }
        auto& log = it->second;
        FileSystem::CreateDirectory(Config::Get().LogPath);
        log.Stream.open(Config::Get().LogPath / (log_name + ".txt"));
        if (!log.Stream.is_open()) {
            streams_.erase(it);
            return false;
        }
        return true;
    }
    S_METHOD_IMPL(Unit, Close, (const std::string& log_name), (log_name)) {
        streams_.erase(log_name);
        return Unit();
    }

    S_METHOD_IMPL(Unit, Log, (const std::string& log, Record&& record), (log, std::move(record))) {
        auto it = streams_.find(log);
        if (it == streams_.end()) {
            return Unit();
        }
        auto& stream = it->second;

        stream.Records.push_back(std::move(record));
        if (stream.Records.size() > stream.Depth) {
            stream.Records.pop_front();
        }
        auto log_string = stream.Records.back().ToString();
        Console::PushLog(log_string);
        std::cout << log_string;
        stream.Stream << log_string;
        stream.Stream.flush();
        if (stream.Records.back().Type == RecordType::Fatal) {
            ITerminate();
        }
        return Unit();
    }
    S_METHOD_IMPL(const std::deque<Logger::Record>&, GetRecords, (const std::string& log), (log)) {
        if (!streams_.contains(log)) {
            return empty_;
        }
        return streams_[log].Records;
    }
    S_METHOD_IMPL(Unit, SetLogDepth, (U32 depth, const std::string& log), (depth, log)) {
        if (!streams_.contains(log)) {
            return Unit();
        }
        streams_[log].Depth = depth;
        return Unit();
    }

    void LogWithAuthor(const std::string& log, Logger::RecordType type, const std::string& author, std::string&& message) {
        Logger::Record record;
        record.Author    = author;
        record.Timestamp = Logger::Record::Clock::now();
        record.Type      = type;
        record.Message   = std::move(message);

        Logger::Log(log, std::move(record));
    }

    void Log(const std::string& log, Logger::RecordType type, std::string&& message) {
        LogWithAuthor(log, type, Logger::kDefaultAuthor, std::move(message));
    }

}  // namespace DummyEngine