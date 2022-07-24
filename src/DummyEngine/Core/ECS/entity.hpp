#pragma once

#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Core/ECS/ComponentManager.hpp"
#include "DummyEngine/Core/ECS/EntityManager.hpp"

namespace DE {

class Entity {
private:
    EntityId m_ID;

public:
    Entity() {
        auto [id, created] = EntityManager::Get().CreateEntity();
        m_ID = id;

        if (created) {
            ComponentManager::Get().ExtendArrays();
        }

        // Logger::Info("ECS", "Entity", "Created by constructor");
    }
    Entity(const Entity& other) {
        m_ID = EntityManager::Get().CopyEntity(other.m_ID);
        // Logger::Info("ECS", "Entity", "Created by copy constructor");
    }
    Entity(Entity&& other) {
        m_ID = EntityManager::Get().CopyEntity(other.m_ID);
        // Logger::Info("ECS", "Entity", "Created by move constructor");
    }
    ~Entity() {
        if (EntityManager::Get().m_Initialized) {
            EntityManager::Get().DestroyInstance(m_ID);
        }
        // Logger::Info("ECS", "Entity", "Destructed.");
    }
    Entity& operator=(const Entity& other) {
        if (&other == this) {
            return *this;
        }
        EntityManager::Get().DestroyInstance(m_ID);
        m_ID = EntityManager::Get().CopyEntity(other.m_ID);
        // Logger::Info("ECS", "Entity", "Assigned by operator=");
        return *this;
    }
    Entity& operator=(Entity&& other) {
        if (&other == this) {
            return *this;
        }
        EntityManager::Get().DestroyInstance(m_ID);
        m_ID = EntityManager::Get().CopyEntity(other.m_ID);
        // Logger::Info("ECS", "Entity", "Assigned by move operator=");
        return *this;
    }

    template <typename ComponentType>
    void AddComponent(ComponentType component = ComponentType()) {
        if (ComponentManager::Get().GetComponentId<ComponentType>() == -1) {
            EntityManager::Get().ExtendSignatures();
        }
        ComponentManager::Get().AddComponent(m_ID, component);
        EntityManager::Get().AddComponent(m_ID, ComponentManager::Get().GetComponentId<ComponentType>());
    }
    template <typename ComponentType>
    void RemoveComponent() {
        if (ComponentManager::Get().GetComponentId<ComponentType>() == -1) {
            EntityManager::Get().ExtendSignatures();
        }
        ComponentManager::Get().RemoveComponent<ComponentType>(m_ID);
        EntityManager::Get().RemoveComponent(m_ID, ComponentManager::Get().GetComponentId<ComponentType>());
    }

    template <typename ComponentType>
    ComponentType& GetComponent() {
        if (ComponentManager::Get().GetComponentId<ComponentType>() == -1) {
            EntityManager::Get().ExtendSignatures();
            ComponentManager::Get().RegisterComponent<ComponentType>();
        }
        if (!EntityManager::Get().GetComponent(m_ID, ComponentManager::Get().GetComponentId<ComponentType>())) {
            ComponentManager::Get().AddComponent<ComponentType>(m_ID);
            EntityManager::Get().AddComponent(m_ID, ComponentManager::Get().GetComponentId<ComponentType>());
        }
        return ComponentManager::Get().GetComponent<ComponentType>(m_ID);
    }

    static void Log() {
        EntityManager::Get().LogState();
        ComponentManager::Get().LogState();
    }
};
}  // namespace DE
