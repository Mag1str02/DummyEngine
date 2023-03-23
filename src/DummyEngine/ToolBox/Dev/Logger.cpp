#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Utils/Config.h"
#include "DummyEngine/Utils/FileSystem.h"
#include "DummyEngine/Utils/Constants.h"
#include "DummyEngine/Utils/Assert.h"
#include "DummyEngine/Utils/StringOperations.h"

namespace DE
{
    std::string LogMessageTypeToStr(LogMessageType type)
    {
        switch (type)
        {
            case LogMessageType::Debug: return "Debug";
            case LogMessageType::Info: return "Info";
            case LogMessageType::Warning: return "Warning";
            case LogMessageType::Error: return "Error";
            case LogMessageType::Fatal: return "Fatal";
            default: return "None";
        }
    }

    static void PrintValue(std::stringstream& out, const int& value)
    {
        if (value > 9)
        {
            out << value;
            return;
        }
        out << '0' << value;
    }
    static void PrintTime(std::stringstream& out, time_t time)
    {
        tm* current_time = localtime(&time);
        out << '[' << current_time->tm_year + 1900 << '.';
        PrintValue(out, current_time->tm_mon + 1);
        out << '.';
        PrintValue(out, current_time->tm_mday);
        out << " | ";
        PrintValue(out, current_time->tm_hour);
        out << '.';
        PrintValue(out, current_time->tm_min);
        out << '.';
        PrintValue(out, current_time->tm_sec);
        out << "] ";
    }

    std::string LogRecord::ToString() const
    {
        std::stringstream ss;
        PrintTime(ss, time);
        ss << std::setw(Constants::MaxMessageTypeLength) << ("[" + LogMessageTypeToStr(type) + "]") << " " << std::left
           << std::setw(Constants::MaxAuthorLength) << StrCat("[", author, "] ") << message << std::endl;
        return ss.str();
    }

    SINGLETON_BASE(Logger);

    Unit Logger::Initialize()
    {
        DE_ASSERT(!s_Instance, "Double logger initialization");
        s_Instance = new Logger();
        DE_ASSERT(s_Instance, "Failed to allocate memory for Logger");

        s_Instance->IInitialize();
        return Unit();
    }
    Unit Logger::IInitialize()
    {
        FileSystem::CreateDirectory(Config::GetPath(DE_CFG_LOG_PATH));
        m_Streams[""].stream.open(Config::GetPath(DE_CFG_LOG_PATH) / "log.txt");
        if (m_Streams[""].stream.is_open())
        {
            std::cout << "Opened log: log.txt" << std::endl;
        }
        else
        {
            std::cout << "Failed to open log: log.txt" << std::endl;
        }
        return Unit();
    }
    Unit Logger::Terminate()
    {
        s_Instance->ITerminate();
        delete s_Instance;
        return Unit();
    }
    Unit Logger::ITerminate()
    {
        for (auto& [name, stream] : m_Streams)
        {
            stream.stream.close();
        }
        m_Streams.clear();
        return Unit();
    }

    S_METHOD_IMPL(Logger, bool, Open, (const std::string& log_name), (log_name))
    {
        if (m_Streams.contains(log_name))
        {
            return false;
        }
        FileSystem::CreateDirectory(Config::GetPath(DE_CFG_LOG_PATH));
        m_Streams[log_name].stream.open(Config::GetPath(DE_CFG_LOG_PATH) / (log_name + ".txt"));
        if (!m_Streams[log_name].stream.is_open())
        {
            m_Streams.erase(log_name);
            return false;
        }
        return true;
    }
    S_METHOD_IMPL(Logger, Unit, Close, (const std::string& log_name), (log_name))
    {
        m_Streams.erase(log_name);
        return Unit();
    }

    void Logger::LogInternal(LogMessageType type, const std::string& author, const std::string& to, const std::string& str)
    {
        LogStream* log = &m_Streams[""];
        if (m_Streams.contains(to))
        {
            log = &m_Streams[to];
        }
        log->records.push_back({time(0), type, author, str});
        if (log->records.size() > log->depth)
        {
            log->records.pop_front();
        }
        log->stream << log->records.back().ToString();
        log->stream.flush();
        if (type == LogMessageType::Fatal)
        {
            ITerminate();
        }
    }
    S_METHOD_IMPL(Logger, const std::deque<LogRecord>&, GetLog, (const std::string& log), (log))
    {
        if (!m_Streams.contains(log))
        {
            return m_Empty;
        }
        return m_Streams[log].records;
    }
    S_METHOD_IMPL(Logger, Unit, SetDepth, (uint32_t depth, const std::string& log), (depth, log))
    {
        if (!m_Streams.contains(log))
        {
            return Unit();
        }
        m_Streams[log].depth = depth;
        return Unit();
    }
}  // namespace DE