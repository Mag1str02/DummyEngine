#pragma once

#include <bitset>
#include <queue>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Addition/const.h"
#include "../../Initializer/initializer.h"
#include "../Component/component_manager.hpp"

namespace DE {

class EntityManager {
private:
    using Signature = std::bitset<MAX_COMPONENT_AMOUNT>;

    friend class Initializer;

    bool _valid;
    std::queue<EntityId> _available_entities;
    std::vector<uint16_t> _ref_count;
    std::vector<Signature> _signatures;
    uint64_t _entities_amount;

public:
    EntityManager() {
        _valid = false;
    }

    EntityId CreateEntity() {
        if (_available_entities.empty()) {
            throw NotEnoughAvailableEntities();
        }

        EntityId entity_id = _available_entities.front();
        _ref_count[entity_id] = 1;
        _available_entities.pop();
        ++_entities_amount;

        Logger::Info("ECS", "EntityManager", "Entity created (" + std::to_string(entity_id) + ")");
        Logger::Info("ECS", "EntityManager", "Entity instance created (" + std::to_string(entity_id) + ")");

        return entity_id;
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

    void Initialize() {
        _ref_count.assign(Initializer::Get()._max_entity_amount, 0);
        _signatures.assign(Initializer::Get()._max_entity_amount, Signature());

        for (size_t i = 0; i < _ref_count.size(); ++i) {
            _available_entities.push(i);
        }

        _entities_amount = 0;
        _valid = true;

        Logger::Info("ECS", "EntityManager", "EntityManager created with max entities amount (" + std::to_string(_ref_count.size()) + ")");
    }
    void Terminate() {
        for (size_t i = 0; i < _ref_count.size(); ++i) {
            if (_ref_count[i]) {
                DestroyEntity(i);
            }
        }

        _valid = false;

        Logger::Info("ECS", "EntityManager", "EntityManager terminated.");
    }

    void LogState() {
        std::string log = "Current State.\n-----------------------------------------------------------------------------------------------\n";
        log.append("Entities amount (" + std::to_string(_entities_amount) + ")\n");
        log.append("Existing entities and their reference count:\n");
        for (size_t i = 0; i < _ref_count.size(); ++i) {
            if (_ref_count[i] > 0) {
                log.append("Id (" + std::to_string(i) + ") | Ref Count (" + std::to_string(_ref_count[i]) + ")\n");
            }
        }
        log.append("-----------------------------------------------------------------------------------------------");
        Logger::Info("ECS", "EntityManager", log);
    }
};
}  // namespace DE