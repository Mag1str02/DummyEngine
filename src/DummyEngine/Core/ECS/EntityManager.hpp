#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"

namespace DE {

class EntityManager {
private:
    using Signature = std::vector<bool>;

    friend class Entity;
    friend class Initializer;

    bool m_Initialized;
    uint64_t m_EntitiesAmount;
    uint64_t m_ComponentAmount;
    std::queue<EntityId> m_AvailableEntities;
    std::vector<uint16_t> m_RefCount;
    std::vector<Signature> m_Signatures;

    EntityManager() {
        m_Initialized = false;
    }

    std::pair<EntityId, bool> CreateEntity() {
        bool new_entity_created = false;
        if (m_AvailableEntities.empty()) {
            m_AvailableEntities.push(m_Signatures.size());
            m_RefCount.push_back(0);
            m_Signatures.push_back(Signature(m_ComponentAmount, false));
            new_entity_created = true;
        }

        EntityId entity_id = m_AvailableEntities.front();
        m_RefCount[entity_id] = 1;
        m_AvailableEntities.pop();
        ++m_EntitiesAmount;

        Logger::Info("ECS", "EntityManager", "Entity created (" + std::to_string(entity_id) + ")");
        Logger::Info("ECS", "EntityManager", "Entity instance created (" + std::to_string(entity_id) + ")");

        return std::make_pair(entity_id, new_entity_created);
    }
    EntityId CopyEntity(EntityId entity_id) {
        ++m_RefCount[entity_id];
        Logger::Info("ECS", "EntityManager", "Entity instance created (" + std::to_string(entity_id) + ")");
        return entity_id;
    }
    bool DestroyInstance(EntityId entity_id) {
        --m_RefCount[entity_id];

        Logger::Info("ECS", "EntityManager", "Entity instance destroyed (" + std::to_string(entity_id) + ")");

        if (!m_RefCount[entity_id]) {
            DestroyEntity(entity_id);
            return true;
        }
        return false;
    }
    void DestroyEntity(EntityId entity_id) {
        m_AvailableEntities.push(entity_id);
        --m_EntitiesAmount;

        Logger::Info("ECS", "EntityManager", "Entity destroyed (" + std::to_string(entity_id) + ")");
    }

    void AddComponent(EntityId entity_id, ComponentId component_id) {
        m_Signatures[entity_id][component_id] = 1;
    }
    void RemoveComponent(EntityId entity_id, ComponentId component_id) {
        m_Signatures[entity_id][component_id] = 0;
    }
    bool GetComponent(EntityId entity_id, ComponentId component_id) {
        return m_Signatures[entity_id][component_id];
    }

    void ExtendSignatures() {
        ++m_ComponentAmount;
        for (auto& signature : m_Signatures) {
            signature.push_back(false);
        }
    }

    void Initialize() {
        m_Initialized = true;
        m_EntitiesAmount = 0;
        m_ComponentAmount = 0;

        Logger::Info("ECS", "EntityManager", "EntityManager created with max entities amount (" + std::to_string(m_EntitiesAmount) + ")");
    }
    void Terminate() {
        m_Initialized = false;

        while (!m_AvailableEntities.empty()) {
            m_AvailableEntities.pop();
        }
        m_RefCount.clear();
        m_Signatures.clear();
        m_EntitiesAmount = 0;
        m_ComponentAmount = 0;

        Logger::Info("ECS", "EntityManager", "EntityManager terminated.");
    }

public:
    static EntityManager& Get() {
        static EntityManager entity_manager;
        return entity_manager;
    }

    void LogState() {
        std::string log = "Current State.\n-----------------------------------------------------------------------------------------------\n";
        log.append("Entities amount (" + std::to_string(m_EntitiesAmount) + ")\n");
        log.append("Existing entities and their reference count:\n");
        for (size_t i = 0; i < m_RefCount.size(); ++i) {
            if (m_RefCount[i] > 0) {
                log.append("Id (" + std::to_string(i) + ") | Ref Count (" + std::to_string(m_RefCount[i]) + ") | Signature (");
                for (const auto& b : m_Signatures[i]) {
                    log.append((b ? "1" : "0"));
                }
                log.append(")\n");
            }
        }
        log.append("-----------------------------------------------------------------------------------------------");
        Logger::Info("ECS", "EntityManager", log);
    }
};
}  // namespace DE