#pragma once

#include "Addition/Base.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{

    class EntityManager
    {
    private:
        using Signature = std::vector<bool>;

        uint64_t m_EntitiesAmount;
        uint64_t m_ComponentAmount;
        std::queue<EntityId> m_AvailableEntities;
        std::vector<Signature> m_Signatures;

    public:
        EntityManager() : m_EntitiesAmount(0), m_ComponentAmount(0)
        {
            Logger::Info("ECS", "EntityManager", "EntityManager created.");
        }
        ~EntityManager()
        {
            Logger::Info("ECS", "EntityManager", "EntityManager terminated.");
        }

        std::pair<EntityId, bool> CreateEntity()
        {
            bool new_entity_created = false;
            if (m_AvailableEntities.empty())
            {
                m_AvailableEntities.push(m_Signatures.size());
                m_Signatures.push_back(Signature(m_ComponentAmount, false));
                new_entity_created = true;
            }

            EntityId entity_id = m_AvailableEntities.front();
            m_AvailableEntities.pop();
            ++m_EntitiesAmount;

            Logger::Info("ECS", "EntityManager", "Entity created (" + std::to_string(entity_id) + ")");

            return std::make_pair(entity_id, new_entity_created);
        }
        void DestroyEntity(EntityId entity_id)
        {
            m_AvailableEntities.push(entity_id);
            --m_EntitiesAmount;

            Logger::Info("ECS", "EntityManager", "Entity destroyed (" + std::to_string(entity_id) + ")");
        }

        void AddComponent(EntityId entity_id, ComponentId component_id)
        {
            m_Signatures[entity_id][component_id] = 1;
        }
        void RemoveComponent(EntityId entity_id, ComponentId component_id)
        {
            m_Signatures[entity_id][component_id] = 0;
        }
        bool GetComponent(EntityId entity_id, ComponentId component_id)
        {
            return m_Signatures[entity_id][component_id];
        }

        void CopyEntity(EntityId from, EntityId to){
            m_Signatures[to] = m_Signatures[from];
        }

        void ExtendSignatures()
        {
            ++m_ComponentAmount;
            for (auto& signature : m_Signatures)
            {
                signature.push_back(false);
            }
        }
    };
}  // namespace DE