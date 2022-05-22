#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "../../../ToolBox/Dev/FrameTimeReader/frame_time_reader.h"
#include "../../../Addition/types.h"
#include "system.hpp"

namespace DE {
class SystemManager {
private:
    std::unordered_map<SystemId, std::shared_ptr<System>> _system_array;
    std::unordered_map<std::string, SystemId> _system_id;
    std::vector<std::vector<SystemId>> _order_graph;
    std::vector<std::vector<SystemId>> _processing_layers;
    size_t _max_layer_width;

    std::vector<double> _avarage_frame_time;
    size_t _frame_amount;

    SystemManager() {
    }

    static SystemManager& Get() {
        static SystemManager static_manager;
        return static_manager;
    }

    void IUpdate(double dt) {
        double prev_time;
        for (const auto& layer : _processing_layers) {
            for (const auto& system : layer) {
                prev_time = glfwGetTime();
                _system_array[system]->Update(dt);
                _avarage_frame_time[system] = (_avarage_frame_time[system] * _frame_amount + glfwGetTime() - prev_time) / (_frame_amount + 1);
            }
        }
        ++_frame_amount;
    }
    template <typename Before, typename After>
    void IAddOrder() {
        _order_graph[_system_id[typeid(Before).name()]].push_back(_system_id[typeid(After).name()]);
    }
    void IClearOrder() {
        _order_graph.assign(_system_id.size(), std::vector<SystemId>());
    }
    void ICalculateOrder() {
        _processing_layers.clear();
        _processing_layers.push_back(std::vector<SystemId>());
        for (size_t i = 0; i < _system_id.size(); ++i) {
            _processing_layers[0].push_back(i);
        }
        _max_layer_width = _system_id.size();
        _avarage_frame_time.resize(_system_array.size());
        _frame_amount = 0;
    }
    template <typename SystemType>
    void IRegisterSystem() {
        if (_system_id.find(typeid(SystemType).name()) == _system_id.end()) {
            _system_array[_system_id.size()] = std::make_shared<SystemType>();
            _system_id[typeid(SystemType).name()] = _system_id.size();
            _order_graph.push_back(std::vector<SystemId>());
        }
    }

    void ITerminate() {
        _system_array.clear();
        _system_id.clear();
        _order_graph.clear();
        for (size_t i = 0; i < _avarage_frame_time.size(); ++i) {
            Logger::Info("rendering", "SystemManager", "System (" + std::to_string(i) + ") avarage frame time (" + std::to_string(_avarage_frame_time[i]) + "s)");
        }
    }

public:
    static void Update(double dt) {
        Get().IUpdate(dt);
    }

    template <typename Before, typename After>
    static void AddOrder() {
        Get().IAddOrder<Before, After>();
    }
    static void ClearOrder() {
        Get().IClearOrder();
    }
    static void CalculateOrder() {
        Get().ICalculateOrder();
    }

    template <typename SystemType>
    static void RegisterSystem() {
        Get().IRegisterSystem<SystemType>();
    }

    static void Terminate() {
        Get().ITerminate();
    }
};
}  // namespace DE