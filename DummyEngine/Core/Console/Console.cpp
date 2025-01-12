#include "Console.hpp"

namespace DummyEngine {

    SINGLETON_BASE(Console);
    S_INITIALIZE() {
        commands_                        = {"clear", "exit", "r_shadowmap_resize"};
        variables_["r_shadowmap_width"]  = 1920ll;
        variables_["r_shadowmap_height"] = 1080ll;
        variables_["r_shadowmap_fbo"]    = false;
        variables_["r_psm_size"]         = 2048ll;
        variables_["r_psm_near"]         = 0.1f;
        variables_["r_psm_far"]          = 50.f;
        variables_["testInt"]            = 534ll;
        variables_["testBool"]           = true;
        variables_["testFloat"]          = 0.5f;
        variables_["testDouble"]         = .234;
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(void, ExecuteCommand, (std::string & cmd), (cmd)) {
        cmd_history_.push_back(cmd);
        if (cmd.find(' ') != std::string::npos) {
            size_t      pos      = cmd.find(' ');
            const auto& var_name = cmd.substr(0, pos);
            if (variables_.find(var_name) != variables_.end()) {
                auto&       value     = variables_[var_name];
                std::string new_value = cmd.substr(pos + 1);
                if (std::holds_alternative<std::string>(value)) {
                    variables_[var_name] = new_value;
                } else if (std::holds_alternative<std::int64_t>(value)) {
                    int64_t unmarshalled = std::stoll(new_value);
                    variables_[var_name] = unmarshalled;
                } else if (std::holds_alternative<float>(value)) {
                    float unmarshalled   = std::stof(new_value);
                    variables_[var_name] = unmarshalled;
                } else if (std::holds_alternative<double>(value)) {
                    double unmarshalled  = std::stod(new_value);
                    variables_[var_name] = unmarshalled;
                } else if (std::holds_alternative<bool>(value)) {
                    if (new_value == "true") {
                        variables_[var_name] = true;
                    } else if (new_value == "false") {
                        variables_[var_name] = false;
                    } else {
                        int64_t unmarshalled = std::stoll(new_value);
                        variables_[var_name] = unmarshalled != 0 ? true : false;
                    }
                } else {
                    LOG_ERROR("Unknown type in cmd {}", cmd);
                }
            }
        }
        if (cmd == "clear") {
            ClearLogs();
        }
        if (callback_.find(cmd) != callback_.end()) {
            callback_[cmd]();
        }
    }

    S_METHOD_IMPL(void, OnCommand, (std::string cmd, std::function<void(void)> func), (cmd, func)) {
        callback_[cmd] = func;
    }

    S_METHOD_IMPL(void, PushLog, (std::string log), (log)) {
        log_history_.push_back(log);
    }

    S_METHOD_IMPL(void, ClearLogs, (), ()) {
        log_history_.clear();
        log_history_.shrink_to_fit();
    }

    S_METHOD_IMPL(std::vector<std::string>&, GetLogHistory, (), ()) {
        return log_history_;
    }

    S_METHOD_IMPL(std::vector<std::string>&, GetCmdHistory, (), ()) {
        return cmd_history_;
    }

    S_METHOD_IMPL(std::string, VarToString, (const std::string& var), (var)) {
        if (variables_.find(var) == variables_.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return "undefined";
        }
        auto& value = variables_[var];
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
        if (variables_.find(var) == variables_.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return -1;
        }
        return std::get<int64_t>(variables_[var]);
    }
    S_METHOD_IMPL(float, GetFloat, (std::string var), (var)) {
        if (variables_.find(var) == variables_.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return -1;
        }
        return std::get<float>(variables_[var]);
    }
    S_METHOD_IMPL(double, GetDouble, (std::string var), (var)) {
        if (variables_.find(var) == variables_.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return -1;
        }
        return std::get<double>(variables_[var]);
    }
    S_METHOD_IMPL(std::string, GetString, (std::string var), (var)) {
        if (variables_.find(var) == variables_.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return "undefined";
        }
        return std::get<std::string>(variables_[var]);
    }

    S_METHOD_IMPL(bool, GetBool, (std::string var), (var)) {
        if (variables_.find(var) == variables_.end()) {
            LOG_WARNING("Undefined variable {}", var);
            return false;
        }
        return std::get<bool>(variables_[var]);
    }

    S_METHOD_IMPL(std::vector<std::string>, GetHints, (std::string & cmd), (cmd)) {
        std::vector<std::string> candidates(commands_);
        for (const auto& [key, val] : variables_) {
            candidates.push_back(key + " " + VarToString(key));
        }
        std::vector<std::string> result;
        int                      space_idx = cmd.find(' ');
        const auto&              trunc_cmd = space_idx == -1 ? cmd : cmd.substr(0, space_idx);  // for variables
        for (const auto& str : candidates) {
            if (str.find(trunc_cmd) != std::string::npos) {
                result.push_back(str);
            }
        }
        return result;
    }
}  // namespace DummyEngine