#define once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {
    using ConsoleVariable = std::variant<int64_t, float, double, bool, std::string>;

    class Console : public Singleton<Console> {
        LOGGER_AUTHOR(Console)
        SINGLETON(Console)
    private:
        S_METHOD_DEF(std::string, VarToString, (const std::string& var))
    public:
        S_METHOD_DEF(void, ExecuteCommand, (std::string & cmd))
        S_METHOD_DEF(void, OnCommand, (std::string cmd, std::function<void(void)> func))
        S_METHOD_DEF(void, PushLog, (std::string log))
        S_METHOD_DEF(std::vector<std::string>&, GetLogHistory, ())
        S_METHOD_DEF(std::vector<std::string>&, GetCmdHistory, ())
        S_METHOD_DEF(void, ClearLogs, ())
        S_METHOD_DEF(int64_t, GetInt, (std::string var))
        S_METHOD_DEF(float, GetFloat, (std::string var))
        S_METHOD_DEF(double, GetDouble, (std::string var))
        S_METHOD_DEF(bool, GetBool, (std::string var))
        S_METHOD_DEF(std::string, GetString, (std::string var))
        S_METHOD_DEF(std::vector<std::string>, GetHints, (std::string& cmd))
    private:
        std::vector<std::string> m_CmdHistory;
        std::vector<std::string> m_LogHistory;
        std::map<std::string, ConsoleVariable> m_Variables;
        std::map<std::string, std::function<void(void)>> m_Callback;
        std::vector<std::string> m_Commands;
    };
}  // namespace DE