#pragma once

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Addition/types.h"
#include "../ECSEngine/ecs_engine.hpp"

namespace DE {

class Entity {
private:
    friend class EntityManager;
    EntityId _id;

public:
    Entity() {
        _id = ECSEngine::Get().CreateEntity();
        // Logger::Info("ECS", "Entity", "Created by constructor");
    }
    Entity(const Entity& other) {
        _id = ECSEngine::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Created by copy constructor");
    }
    Entity(Entity&& other) {
        _id = ECSEngine::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Created by move constructor");
    }
    Entity& operator=(const Entity& other) {
        if (&other == this) {
            return *this;
        }
        _id = ECSEngine::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Assigned by operator=");
        return *this;
    }
    Entity& operator=(Entity&& other) {
        if (&other == this) {
            return *this;
        }
        _id = ECSEngine::Get().CopyEntity(other._id);
        // Logger::Info("ECS", "Entity", "Assigned by move operator=");
        return *this;
    }
    ~Entity() {
        if (EntityManager::Get()._valid) {
            EntityManager::Get().DestroyInstance(_id);
        }
        // Logger::Info("ECS", "Entity", "Destructed.");
    }

    template <typename ComponentType>
    void AddComponent(ComponentType component = ComponentType()) {
        ComponentManager::Get().AddComponent(_id, component);
    }
    template <typename ComponentType>
    void RemoveComponent() {
        ComponentManager::Get().RemoveComponent<ComponentType>(_id);
    }

    template <typename ComponentType>
    ComponentType& GetComponent() {
        return ComponentManager::Get().GetComponent<ComponentType>(_id);
    }
};
}  // namespace DE
