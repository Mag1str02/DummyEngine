//
// Created by balanda on 9/29/2023.
//

#include "Console.hpp"

namespace DE {
    SINGLETON_BASE(Console);
    S_INITIALIZE() {
        m_Commands                        = {"clear", "exit", "r_shadowmap_resize"};
        m_Variables["r_shadowmap_width"]  = 1920ll;
        m_Variables["r_shadowmap_height"] = 1080ll;
        m_Variables["r_shadowmap_fbo"]    = false;
        m_Variables["r_psm_size"]         = 2048ll;
        m_Variables["r_psm_near"]         = 0.1f;
        m_Variables["r_psm_far"]          = 50.f;
        m_Variables["testInt"]            = 534ll;
        m_Variables["testBool"]           = true;
        m_Variables["testFloat"]          = 0.5f;
        m_Variables["testDouble"]         = .234;
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(void, ExecuteCommand, (std::string & cmd), (cmd)) {
        m_CmdHistory.push_back(cmd);
        if (cmd.find(' ') != -1) {
            size_t      pos      = cmd.find(' ');
            const auto& var_name = cmd.substr(0, pos);
            if (m_Variables.find(var_name) != m_Variables.end()) {
                auto&       value     = m_Variables[var_name];
                std::string new_value = cmd.substr(pos + 1);
                if (std::holds_alternative<std::string>(value)) {
                    m_Variables[var_name] = new_value;
                } else if (std::holds_alternative<std::int64_t>(value)) {
                    int64_t unmarshalled  = std::stoll(new_value);
                    m_Variables[var_name] = unmarshalled;
                } else if (std::holds_alternative<float>(value)) {
                    float unmarshalled    = std::stof(new_value);
                    m_Variables[var_name] = unmarshalled;
                } else if (std::holds_alternative<double>(value)) {
                    double unmarshalled   = std::stod(new_value);
                    m_Variables[var_name] = unmarshalled;
                } else if (std::holds_alternative<bool>(value)) {
                    if (new_value == "true") {
                        m_Variables[var_name] = true;
                    } else if (new_value == "false") {
                        m_Variables[var_name] = false;
                    } else {
                        int64_t unmarshalled  = std::stoll(new_value);
                        m_Variables[var_name] = unmarshalled ? true : false;
                    }
                } else {
                    LOG_ERROR("Unknown type in cmd {}", cmd);
                }
            }
        }
        if (cmd == "clear") {
            ClearLogs();
        }
        if (m_Callback.find(cmd) != m_Callback.end()) {
            m_Callback[cmd]();
        }
    }

    S_METHOD_IMPL(void, OnCommand, (std::string cmd, std::function<void(void)> func), (cmd, func)) {
        m_Callback[cmd] = func;
    }

    S_METHOD_IMPL(void, PushLog, (std::string log), (log)) {
        m_LogHistory.push_back(log);
    }

    S_METHOD_IMPL(void, ClearLogs, (), ()) {
        m_LogHistory.clear();
        m_LogHistory.shrink_to_fit();
    }

    S_METHOD_IMPL(std::vector<std::string>&, GetLogHistory, (), ()) {
        return m_LogHistory;
    }

    S_METHOD_IMPL(std::vector<std::string>&, GetCmdHistory, (), ()) {
        return m_CmdHistory;
    }

    S_METHOD_IMPL(std::string, VarToString, (const std::string& var), (var)) {
        if (m_Variables.find(var) == m_Variables.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return "undefined";
        }
        auto& value = m_Variables[var];
        if (std::holds_alternative<std::string>(value)) {
            return std::get<std::string>(value);
        } else if (std::holds_alternative<int64_t>(value)) {
            return std::to_string(std::get<int64_t>(value));
        } else if (std::holds_alternative<float>(value)) {
            return std::to_string(std::get<float>(value));
        } else if (std::holds_alternative<double>(value)) {
            return std::to_string(std::get<double>(value));
        } else if (std::holds_alternative<bool>(value)) {
            return (std::get<bool>(value) ? "true" : "false");
        }
        LOG_WARNING("Unknown type of variable {}", var);
        return "unknown type";
    }

    S_METHOD_IMPL(int64_t, GetInt, (std::string var), (var)) {
        if (m_Variables.find(var) == m_Variables.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return -1;
        }
        return std::get<int64_t>(m_Variables[var]);
    }
    S_METHOD_IMPL(float, GetFloat, (std::string var), (var)) {
        if (m_Variables.find(var) == m_Variables.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return -1;
        }
        return std::get<float>(m_Variables[var]);
    }
    S_METHOD_IMPL(double, GetDouble, (std::string var), (var)) {
        if (m_Variables.find(var) == m_Variables.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return -1;
        }
        return std::get<double>(m_Variables[var]);
    }
    S_METHOD_IMPL(std::string, GetString, (std::string var), (var)) {
        if (m_Variables.find(var) == m_Variables.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return "undefined";
        }
        return std::get<std::string>(m_Variables[var]);
    }

    S_METHOD_IMPL(bool, GetBool, (std::string var), (var)) {
        if (m_Variables.find(var) == m_Variables.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return false;
        }
        return std::get<bool>(m_Variables[var]);
    }

    S_METHOD_IMPL(std::vector<std::string>, GetHints, (std::string & cmd), (cmd)) {
        std::vector<std::string> candidates(m_Commands);
        for (const auto& [key, val] : m_Variables) {
            candidates.push_back(key + " " + VarToString(key));
        }
        std::vector<std::string> result;
        int                      space_idx = cmd.find(' ');
        const auto&              trunc_cmd = space_idx == -1 ? cmd : cmd.substr(0, space_idx);  // for variables
        for (const auto& str : candidates) {
            if (str.find(trunc_cmd) != -1) {
                result.push_back(str);
            }
        }
        return result;
    }
}  // namespace DE