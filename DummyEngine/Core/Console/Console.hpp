#define once

#include "DummyEngine/Utils/Debug/Logger.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

#include <map>

namespace DummyEngine {

    class Console : public Singleton<Console> {
        using Variable = std::variant<S64, float, double, bool, std::string>;

        LOG_AUTHOR(Console)
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
        S_METHOD_DEF(std::vector<std::string>, GetHints, (std::string & cmd))
    private:
        std::vector<std::string>                         cmd_history_;
        std::vector<std::string>                         log_history_;
        std::map<std::string, Variable>                  variables_;
        std::map<std::string, std::function<void(void)>> callback_;
        std::vector<std::string>                         commands_;
    };

}  // namespace DummyEngine