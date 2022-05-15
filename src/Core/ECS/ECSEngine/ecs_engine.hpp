#pragma once

#include "../Component/component_manager.hpp"
#include "../Entity/entity_manager.hpp"

namespace DE {

class ECSEngine {
private:
    friend class Entity;
    bool _valid;
    EntityManager entity_manager;
    ComponentManager component_manager;

    ECSEngine() {
        _valid = false;
    }
    static ECSEngine& Get() {
        static ECSEngine ecs_engine;
        return ecs_engine;
    }

    EntityId CreateEntity() {
        return entity_manager.CreateEntity();
    }
    EntityId CopyEntity(EntityId entity_id) {
        return entity_manager.CopyEntity(entity_id);
    }
    void DestroyInstance(EntityId entity_id) {
        if (_valid) {
            if (entity_manager.DestroyInstance(entity_id)) {
                component_manager.EntityDestroyed(entity_id);
            }
        }
    }
    void DestroyEntity(EntityId entity_id) {
        entity_manager.DestroyInstance(entity_id);
        component_manager.EntityDestroyed(entity_id);
    }

    template <typename ComponentType>
    void AddComponent(EntityId entity_id, ComponentType component) {
        component_manager.AddComponent(entity_id, component);
        entity_manager.AddComponent(entity_id, component_manager.GetComponentId<ComponentType>());
    }
    template <typename ComponentType>
    void RemoveComponent(EntityId entity_id) {
        component_manager.RemoveComponent<ComponentType>(entity_id);
        entity_manager.RemoveComponent(entity_id, component_manager.GetComponentId<ComponentType>());
    }
    template <typename ComponentType>
    ComponentType& GetComponent(EntityId entity_id) {
        return component_manager.GetComponent<ComponentType>(entity_id);
    }

    void Initialize() {
        entity_manager.Initialize();
        _valid = true;
    }
    void Terminate() {
        entity_manager.Terminate();
        _valid = true;
    }
};

}  // namespace DE