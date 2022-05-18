#pragma once

#include <iostream>

#include "../../../Addition/types.h"
#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../Component/component_manager.hpp"
#include "entity_manager.hpp"

namespace DE {

class Entity {
private:
    EntityId _id;

public:
    Entity() {
        _id = EntityManager::Get().CreateEntity();
        // Logger::Info("ECS", "Entity", "Created by constructor");
    }
    Entity(const Entity& other) {
        _id = EntityManager::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Created by copy constructor");
    }
    Entity(Entity&& other) {
        _id = EntityManager::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Created by move constructor");
    }
    ~Entity() {
        if (EntityManager::Get()._valid) {
            EntityManager::Get().DestroyInstance(_id);
        }
        // Logger::Info("ECS", "Entity", "Destructed.");
    }
    Entity& operator=(const Entity& other) {
        if (&other == this) {
            return *this;
        }
        EntityManager::Get().DestroyInstance(_id);
        _id = EntityManager::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Assigned by operator=");
        return *this;
    }
    Entity& operator=(Entity&& other) {
        if (&other == this) {
            return *this;
        }
        EntityManager::Get().DestroyInstance(_id);
        _id = EntityManager::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Assigned by move operator=");
        return *this;
    }

    template <typename ComponentType>
    void AddComponent(ComponentType component = ComponentType()) {
        if (ComponentManager::Get().GetComponentId<ComponentType>() == -1) {
            EntityManager::Get().ExtendSignatures();
        }
        ComponentManager::Get().AddComponent(_id, component);
        EntityManager::Get().AddComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>());
    }
    template <typename ComponentType>
    void RemoveComponent() {
        if (ComponentManager::Get().GetComponentId<ComponentType>() == -1) {
            EntityManager::Get().ExtendSignatures();
        }
        ComponentManager::Get().RemoveComponent<ComponentType>(_id);
        EntityManager::Get().RemoveComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>());
    }

    template <typename ComponentType>
    ComponentType& GetComponent() {
        if (ComponentManager::Get().GetComponentId<ComponentType>() == -1) {
            EntityManager::Get().ExtendSignatures();
            ComponentManager::Get().RegisterComponent<ComponentType>();
        }
        if (!EntityManager::Get().GetComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>())) {
            ComponentManager::Get().AddComponent<ComponentType>(_id);
            EntityManager::Get().AddComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>());
        }
        return ComponentManager::Get().GetComponent<ComponentType>(_id);
    }

    static void Log() {
        EntityManager::Get().LogState();
        ComponentManager::Get().LogState();
    }
};
}  // namespace DE
