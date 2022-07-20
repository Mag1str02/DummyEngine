#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Addition/const.h"
#include "DummyEngine/Addition/types.h"
#include "DummyEngine/ToolBox/Dev/logger.h"

namespace DE {

class EntityManager {
private:
    using Signature = std::vector<bool>;

    friend class Entity;
    friend class Initializer;

    bool _initialized;
    uint64_t _entities_amount;
    uint64_t _component_amount;
    std::queue<EntityId> _available_entities;
    std::vector<uint16_t> _ref_count;
    std::vector<Signature> _signatures;

    EntityManager() {
        _initialized = false;
    }

    std::pair<EntityId, bool> CreateEntity() {
        bool new_entity_created = false;
        if (_available_entities.empty()) {
            _available_entities.push(_signatures.size());
            _ref_count.push_back(0);
            _signatures.push_back(Signature(_component_amount, false));
            new_entity_created = true;
        }

        EntityId entity_id = _available_entities.front();
        _ref_count[entity_id] = 1;
        _available_entities.pop();
        ++_entities_amount;

        Logger::Info("ECS", "EntityManager", "Entity created (" + std::to_string(entity_id) + ")");
        Logger::Info("ECS", "EntityManager", "Entity instance created (" + std::to_string(entity_id) + ")");

        return std::make_pair(entity_id, new_entity_created);
    }
    EntityId CopyEntity(EntityId entity_id) {
        ++_ref_count[entity_id];
        Logger::Info("ECS", "EntityManager", "Entity instance created (" + std::to_string(entity_id) + ")");
        return entity_id;
    }
    bool DestroyInstance(EntityId entity_id) {
        --_ref_count[entity_id];

        Logger::Info("ECS", "EntityManager", "Entity instance destroyed (" + std::to_string(entity_id) + ")");

        if (!_ref_count[entity_id]) {
            DestroyEntity(entity_id);
            return true;
        }
        return false;
    }
    void DestroyEntity(EntityId entity_id) {
        _available_entities.push(entity_id);
        --_entities_amount;

        Logger::Info("ECS", "EntityManager", "Entity destroyed (" + std::to_string(entity_id) + ")");
    }

    void AddComponent(EntityId entity_id, ComponentId component_id) {
        _signatures[entity_id][component_id] = 1;
    }
    void RemoveComponent(EntityId entity_id, ComponentId component_id) {
        _signatures[entity_id][component_id] = 0;
    }
    bool GetComponent(EntityId entity_id, ComponentId component_id) {
        return _signatures[entity_id][component_id];
    }

    void ExtendSignatures() {
        ++_component_amount;
        for (auto& signature : _signatures) {
            signature.push_back(false);
        }
    }

    void Initialize() {
        _initialized = true;
        _entities_amount = 0;
        _component_amount = 0;

        Logger::Info("ECS", "EntityManager", "EntityManager created with max entities amount (" + std::to_string(_entities_amount) + ")");
    }
    void Terminate() {
        _initialized = false;

        while (!_available_entities.empty()) {
            _available_entities.pop();
        }
        _ref_count.clear();
        _signatures.clear();
        _entities_amount = 0;
        _component_amount = 0;

        Logger::Info("ECS", "EntityManager", "EntityManager terminated.");
    }

public:
    static EntityManager& Get() {
        static EntityManager entity_manager;
        return entity_manager;
    }

    void LogState() {
        std::string log = "Current State.\n-----------------------------------------------------------------------------------------------\n";
        log.append("Entities amount (" + std::to_string(_entities_amount) + ")\n");
        log.append("Existing entities and their reference count:\n");
        for (size_t i = 0; i < _ref_count.size(); ++i) {
            if (_ref_count[i] > 0) {
                log.append("Id (" + std::to_string(i) + ") | Ref Count (" + std::to_string(_ref_count[i]) + ") | Signature (");
                for (const auto& b : _signatures[i]) {
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