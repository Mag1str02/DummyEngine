#pragma once

#include <memory>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "component_array.hpp"

namespace DE {
using ComponentId = uint16_t;

class ComponentManager {
private:
    std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _component_arrays;
    std::unordered_map<std::string, ComponentId> _component_type;

    ComponentManager() {
    }

    template <typename ComponentType>
    void RegisterComponent() {
        const char* component_name = typeid(ComponentType).name();
        _component_type[component_name] = _component_type.size();
        _component_arrays[component_name] = std::make_shared<ComponentArray<ComponentType>>();
        Logger::Info("ECS", "ComponentManager", "Registered Component (" + NormalTypeName(component_name) + ")");
    }
    template <typename ComponentType>
    std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray() {
        if (_component_arrays.find(typeid(ComponentType).name()) == _component_arrays.end()) {
            RegisterComponent<ComponentType>();
        }
        return std::static_pointer_cast<ComponentArray<ComponentType>>(_component_arrays[typeid(ComponentType).name()]);
    }

    std::string NormalTypeName(const std::string& s_name) {
        for (size_t i = 1; i < s_name.size(); ++i) {
            if ('0' > s_name[i] || s_name[i] > '9') {
                return s_name.substr(i, s_name.size() - i);
            }
        }
        return s_name;
    }

public:
    static ComponentManager& Get() {
        static ComponentManager component_manager;
        return component_manager;
    }

    template <typename ComponentType>
    void AddComponent(EntityId id, ComponentType component) {
        GetComponentArray<ComponentType>()->InsertComponent(id, component);
        Logger::Info("ECS", "ComponentManager",
                     "Added Component (" + NormalTypeName(typeid(ComponentType).name()) + ") to Entity (" + std::to_string(id) + ")");
    }
    template <typename ComponentType>
    void RemoveComponent(EntityId id) {
        GetComponentArray<ComponentType>()->RemoveComponent(id);
        Logger::Info("ECS", "ComponentManager",
                     "Removed Component " + NormalTypeName(typeid(ComponentType).name()) + " from Entity " + std::to_string(id));
    }
    void EntityDestroyed(EntityId id) {
        for (auto& [name, component_array] : _component_arrays) {
            component_array->EntityDestroyed(id);
        }
    }

    template <typename ComponentType>
    ComponentType* GetComponent(EntityId id) {
        return GetComponentArray<ComponentType>()->GetComponent(id);
    }

    void LogState() {
        std::string log = "Current State.\n-----------------------------------------------------------------------------------------------\n";
        for (const auto& [name, component_array] : _component_arrays) {
            log.append("Component type " + NormalTypeName(name) + "\n");
            log.append(component_array->LogState());
        }
        log.append("-----------------------------------------------------------------------------------------------");
        Logger::Info("ECS", "ComponentManager", log);
    }
};
}  // namespace DE