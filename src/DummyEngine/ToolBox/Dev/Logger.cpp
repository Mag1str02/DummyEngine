#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Utils/Config.h"
#include "DummyEngine/Utils/FileSystem.h"
#include "DummyEngine/Utils/Constants.h"
#include "DummyEngine/Utils/Assert.h"

namespace DE
{
    static void PrintValue(std::ostream& out, const int& value)
    {
        if (value > 9)
        {
            out << value;
            return;
        }
        out << '0' << value;
    }
    static void PrintTime(std::ostream& out)
    {
        time_t now          = time(0);
        tm*    current_time = localtime(&now);
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
    static void Log(std::unordered_map<std::string, std::ofstream>& m_Streams,
                    std::ofstream&                                  m_All,
                    const std::string&                              str,
                    const std::string&                              author,
                    const std::string&                              log,
                    const std::string&                              type)
    {
        std::ofstream* out = &m_All;
        if (log != "" && m_Streams.contains(log))
        {
            out = &m_Streams[log];
        }

        PrintTime(*out);
        *out << std::setw(Constants::MaxMessageTypeLength) << ("[" + type + "]") << " " << author << ": " << str << std::endl;
        return;
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
        m_All.open(Config::GetPath(DE_CFG_LOG_PATH) / "log.txt");
        if (m_All.is_open())
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
            stream.close();
        }
        m_All.close();
        return Unit();
    }

    S_METHOD_IMPL(Logger, bool, Open, (const std::string& log_name), (log_name))
    {
        if (m_Streams.contains(log_name))
        {
            return false;
        }
        FileSystem::CreateDirectory(Config::GetPath(DE_CFG_LOG_PATH));
        m_Streams[log_name].open(Config::GetPath(DE_CFG_LOG_PATH) / log_name);
        if (!m_Streams[log_name].is_open())
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

    S_METHOD_IMPL(Logger, Unit, Error, (const std::string& str, const std::string& author, const std::string& log), (str, author, log))
    {
        Log(m_Streams, m_All, str, author, log, "Error");
        return Unit();
    }
    S_METHOD_IMPL(Logger, Unit, Info, (const std::string& str, const std::string& author, const std::string& log), (str, author, log))
    {
        Log(m_Streams, m_All, str, author, log, "Info");
        return Unit();
    }
    S_METHOD_IMPL(Logger, Unit, Warning, (const std::string& str, const std::string& author, const std::string& log), (str, author, log))
    {
        Log(m_Streams, m_All, str, author, log, "Warning");
        return Unit();
    }
    S_METHOD_IMPL(Logger, Unit, Stage, (const std::string& str, const std::string& author, const std::string& log), (str, author, log))
    {
        Log(m_Streams, m_All, str, author, log, "Stage");
        return Unit();
    }
    S_METHOD_IMPL(Logger, Unit, Fatal, (const std::string& str, const std::string& author, const std::string& log), (str, author, log))
    {
        Log(m_Streams, m_All, str, author, log, "Fatal");
        Terminate();
        return Unit();
    }

}  // namespace DE