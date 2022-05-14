#pragma once

#include <memory>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Addition/some_funcs.h"
#include "component_array.hpp"


namespace DE {

class ComponentManager {
private:
    friend class EntityManager;
    friend class Entity;
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

    template <typename ComponentType>
    void AddComponent(EntityId id, ComponentType component) {
        GetComponentArray<ComponentType>()->InsertComponent(id, component);
    }
    template <typename ComponentType>
    void RemoveComponent(EntityId id) {
        GetComponentArray<ComponentType>()->RemoveComponent(id);
    }
    void EntityDestroyed(EntityId id) {
        for (auto& [name, component_array] : _component_arrays) {
            component_array->EntityDestroyed(id);
        }
    }

    template <typename ComponentType>
    ComponentType& GetComponent(EntityId id) {
        return GetComponentArray<ComponentType>()->GetComponent(id);
    }

public:
    static ComponentManager& Get() {
        static ComponentManager component_manager;
        return component_manager;
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