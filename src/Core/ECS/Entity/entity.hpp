#pragma once

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "entity_manager.hpp"
#include "../Component/component_manager.hpp"
#include "../../Addition/types.h"

namespace DE
{

    class Entity
    {
    private:
        EntityId _id;

    public:
        Entity()
        {
            _id = EntityManager::Get().CreateEntity();
            // Logger::Info("ECS", "Entity", "Created by constructor");
        }
        Entity(const Entity &other)
        {
            _id = EntityManager::Get().CopyEntity(other._id);
            // Logger::Info("ECS", "Entity", "Created by copy constructor");
        }
        Entity(Entity &&other)
        {
            _id = EntityManager::Get().CopyEntity(other._id);
            // Logger::Info("ECS", "Entity", "Created by move constructor");
        }
        Entity &operator=(const Entity &other)
        {
            if (&other == this)
            {
                return *this;
            }
            _id = EntityManager::Get().CopyEntity(other._id);
            // Logger::Info("ECS", "Entity", "Assigned by operator=");
            return *this;
        }
        Entity &operator=(Entity &&other)
        {
            if (&other == this)
            {
                return *this;
            }
            _id = EntityManager::Get().CopyEntity(other._id);
            // Logger::Info("ECS", "Entity", "Assigned by move operator=");
            return *this;
        }
        ~Entity()
        {
            if (EntityManager::Get()._valid)
            {
                EntityManager::Get().DestroyInstance(_id);
            }
            // Logger::Info("ECS", "Entity", "Destructed.");
        }

        template <typename ComponentType>
        void AddComponent(ComponentType component = ComponentType())
        {
            ComponentManager::Get().AddComponent(_id, component);
            EntityManager::Get().AddComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>());
        }
        template <typename ComponentType>
        void RemoveComponent()
        {
            ComponentManager::Get().RemoveComponent<ComponentType>(_id);
            EntityManager::Get().RemoveComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>());
        }

        template <typename ComponentType>
        ComponentType &GetComponent()
        {
            if (!EntityManager::Get().GetComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>()))
            {
                ComponentManager::Get().AddComponent<ComponentType>(_id);
                EntityManager::Get().AddComponent(_id, ComponentManager::Get().GetComponentId<ComponentType>());
            }
            return ComponentManager::Get().GetComponent<ComponentType>(_id);
        }
    };
} // namespace DE
