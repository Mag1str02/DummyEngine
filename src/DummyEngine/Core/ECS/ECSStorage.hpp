#pragma once

#include "Addition/Base.h"

#include "Core/ECS/ComponentManager.hpp"
#include "Core/ECS/EntityManager.hpp"
#include "Core/ECS/SystemManager.hpp"

namespace DE
{
    class ECSStorage
    {
    public:
        ECSStorage() = default;

        EntityId CreateEntity()
        {
            auto [id, created] = m_EntityManager.CreateEntity();

            if (created)
            {
                m_ComponentManager.ExtendArrays();
            }
            return id;
        }
        void DestroyEntity(EntityId id)
        {
            m_EntityManager.DestroyEntity(id);
        }

        template <typename ComponentType>
        ComponentType& AddComponent(EntityId id, ComponentType component = ComponentType())
        {
            if (m_ComponentManager.GetComponentId<ComponentType>() == -1)
            {
                m_EntityManager.ExtendSignatures();
            }
            m_ComponentManager.AddComponent(id, component);
            m_EntityManager.AddComponent(id, m_ComponentManager.GetComponentId<ComponentType>());
            return m_ComponentManager.GetComponent<ComponentType>(id);
        }
        template <typename ComponentType> void RemoveComponent(EntityId id)
        {
            DE_ASSERT(m_ComponentManager.GetComponentId<ComponentType>() != -1,
                      "Attempt to access non existing component arry.");
            DE_ASSERT(m_ComponentManager.GetComponentArray<ComponentType>()->HasComponent(id),
                      "Attempt to delete non existing component.");

            m_ComponentManager.RemoveComponent<ComponentType>(id);
            m_EntityManager.RemoveComponent(id, m_ComponentManager.GetComponentId<ComponentType>());
        }
        template <typename ComponentType> ComponentType& GetComponent(EntityId id)
        {
            DE_ASSERT(m_ComponentManager.GetComponentId<ComponentType>() != -1,
                      "Attempt to access non existing component array.");
            return m_ComponentManager.GetComponent<ComponentType>(id);
        }
        template <typename ComponentType> bool HasComponent(EntityId id)
        {
            return m_ComponentManager.GetComponentArray<ComponentType>()->HasComponent(id);
        }

        template <typename ComponentType> ComponentArray<ComponentType>& GetComponentArray()
        {
            return *m_ComponentManager.GetComponentArray<ComponentType>();
        }

        template <typename SystemType> void RegisterSystem()
        {
            m_SystemManager.RegisterSystem<SystemType>()->Bind(&m_ComponentManager);
        }
        void UpdateSystems(double dt)
        {
            m_SystemManager.Update(dt);
        }

    private:
        EntityManager m_EntityManager;
        ComponentManager m_ComponentManager;
        SystemManager m_SystemManager;
    };
}  // namespace DE