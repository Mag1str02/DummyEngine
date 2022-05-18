#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

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

public:
    static void Update(double dt) {
        double prev_time;
        for (const auto& layer : Get()._processing_layers) {
            for (const auto& system : layer) {
                prev_time = glfwGetTime();
                Get()._system_array[system]->Update(dt);
                Get()._avarage_frame_time[system] = (Get()._avarage_frame_time[system] * Get()._frame_amount + glfwGetTime() - prev_time) / (Get()._frame_amount + 1);
            }
        }
        ++Get()._frame_amount;
    }

    template <typename Before, typename After>
    static void AddOrder() {
        Get()._order_graph[Get()._system_id[typeid(Before).name()]].push_back(Get()._system_id[typeid(After).name()]);
    }
    template <typename Before, typename After>
    static void ClearOrder() {
        Get()._order_graph.assign(Get()._system_id.size(), std::vector<SystemId>());
    }
    static void CalculateOrder() {
        Get()._processing_layers.clear();
        Get()._processing_layers.push_back(std::vector<SystemId>());
        for (size_t i = 0; i < Get()._system_id.size(); ++i) {
            Get()._processing_layers[0].push_back(i);
        }
        Get()._max_layer_width = Get()._system_id.size();
        Get()._avarage_frame_time.resize(Get()._system_array.size());
        Get()._frame_amount = 0;
    }

    template <typename SystemType>
    static void RegisterSystem() {
        if (Get()._system_id.find(typeid(SystemType).name()) == Get()._system_id.end()) {
            Get()._system_array[Get()._system_id.size()] = std::make_shared<SystemType>();
            Get()._system_id[typeid(SystemType).name()] = Get()._system_id.size();
            Get()._order_graph.push_back(std::vector<SystemId>());
        }
    }

    static void Terminate() {
        Get()._system_array.clear();
        Get()._system_id.clear();
        Get()._order_graph.clear();
        for (size_t i = 0; i < Get()._avarage_frame_time.size(); ++i) {
            Logger::Info("rendering", "SystemManager", "System (" + std::to_string(i) + ") avarage frame time (" + std::to_string(Get()._avarage_frame_time[i]) + "s)");
        }
    }
};
}  // namespace DE