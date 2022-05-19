#pragma once

#include <memory>

#include "../../../Addition/some_funcs.h"
#include "../../../ToolBox/Dev/Logger/logger.h"
#include "component_array.hpp"

namespace DE {
class ComponentManager {
private:
    friend class Entity;
    friend class Initializer;

    std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _component_arrays;
    std::unordered_map<std::string, ComponentId> _component_id;
    size_t _entity_amount;

    ComponentManager() {
    }

    template <typename ComponentType>
    void RegisterComponent() {
        const char* component_name = typeid(ComponentType).name();

        _component_id[component_name] = _component_id.size();
        _component_arrays[component_name] = std::make_shared<ComponentArray<ComponentType>>(ComponentArray<ComponentType>(_entity_amount));

        Logger::Info("ECS", "ComponentManager", "Registered Component (" + NormalTypeName(component_name) + ")");
    }

    template <typename ComponentType>
    void AddComponent(EntityId entity_id, ComponentType component = ComponentType()) {
        GetComponentArray<ComponentType>()->InsertComponent(entity_id, component);
    }
    template <typename ComponentType>
    void RemoveComponent(EntityId entity_id) {
        GetComponentArray<ComponentType>()->RemoveComponent(entity_id);
    }
    void EntityDestroyed(EntityId entity_id) {
        for (auto& [name, component_array] : _component_arrays) {
            component_array->EntityDestroyed(entity_id);
        }
    }

    template <typename ComponentType>
    ComponentType& GetComponent(EntityId entity_id) {
        return (*GetComponentArray<ComponentType>())[entity_id];
    }
    template <typename ComponentType>
    ComponentId GetComponentId(ComponentType component = ComponentType()) {
        if (_component_id.find(typeid(ComponentType).name()) != _component_id.end()) {
            return _component_id[typeid(ComponentType).name()];
        }
        return -1;
    }

    void Initialize() {
    }
    void Terminate() {
        _component_arrays.clear();
        _component_id.clear();
    }

    void ExtendArrays() {
        ++_entity_amount;
        for (auto& [name, component_array] : _component_arrays) {
            component_array->ExtendArray();
        }
    }

public:
    static ComponentManager& Get() {
        static ComponentManager component_manager;
        return component_manager;
    }

    template <typename ComponentType>
    std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray() {
        if (_component_arrays.find(typeid(ComponentType).name()) == _component_arrays.end()) {
            RegisterComponent<ComponentType>();
        }
        return std::static_pointer_cast<ComponentArray<ComponentType>>(_component_arrays[typeid(ComponentType).name()]);
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