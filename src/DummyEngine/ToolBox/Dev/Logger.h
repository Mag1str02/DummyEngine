#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <set>
#include <fstream>

#include "DummyEngine/Utils/Types.h"
#include "DummyEngine/Utils/Singleton.h"

namespace DE
{
    class Logger : public Singleton<Logger>
    {
        SINGLETON(Logger)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(bool, Open, (const std::string& log_name));
        S_METHOD_DEF(Unit, Close, (const std::string& log_name));

        S_METHOD_DEF(Unit, Error, (const std::string& str, const std::string& author = "Anonymous", const std::string& log = ""));
        S_METHOD_DEF(Unit, Info, (const std::string& str, const std::string& author = "Anonymous", const std::string& log = ""));
        S_METHOD_DEF(Unit, Warning, (const std::string& str, const std::string& author = "Anonymous", const std::string& log = ""));
        S_METHOD_DEF(Unit, Stage, (const std::string& str, const std::string& author = "Anonymous", const std::string& log = ""));
        S_METHOD_DEF(Unit, Fatal, (const std::string& str, const std::string& author = "Anonymous", const std::string& log = ""));

    private:
        Logger()  = default;
        ~Logger() = default;

        std::unordered_map<std::string, std::ofstream> m_Streams;
        std::ofstream                                  m_All;
    };
}  // namespace DE

#if DE_ENABLE_LOGGING == 1
#define LOG_ERROR(...) Logger::Error(__VA_ARGS__)
#define LOG_INFO(...) Logger::Info(__VA_ARGS__)
#define LOG_WARNING(...) Logger::Warning(__VA_ARGS__)
#define LOG_STAGE(...) Logger::Stage(__VA_ARGS__)
#define LOG_FATAL(...) Logger::Fatal(__VA_ARGS__)
#else
#define LOG_ERROR(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_STAGE(...)
#define LOG_FATAL(...)
#endif