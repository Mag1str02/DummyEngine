#pragma once

#include <queue>
#include <iostream>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Initializer/initializer.h"
#include "../Component/component_manager.hpp"

namespace DE {

using EntityId = int64_t;

class EntityManager {
private:
    std::queue<EntityId> _available_entities;
    std::vector<bool> _available;
    uint64_t _entities_amount;

    uint64_t _max_entities_amount;

    EntityManager() {
    }

public:
    static EntityManager& Get() {
        static EntityManager entity_manager;
        return entity_manager;
    }

    EntityId CreateEntity() {
        if (_available_entities.empty()) {
           throw std::overflow_error("No available entity id.");
        }
        EntityId new_entity = _available_entities.front();
        _available_entities.pop();
        _available[new_entity] = false;
        ++_entities_amount;
        Logger::Info("ECS", "EntityManager", "Entity created (" + std::to_string(new_entity) + ")");
        return new_entity;
    }
    void DestroyEntity(EntityId entity) {
        if (entity < 0 || entity >= _max_entities_amount || _available[entity]) {
            return;
        }
        _available[entity] = true;
        --_entities_amount;
        _available_entities.push(entity);
        ComponentManager::Get().EntityDestroyed(entity);
        Logger::Info("ECS", "EntityManager", "Entity destroyed (" + std::to_string(entity) + ")");
    }
    void Initialize() {
        _max_entities_amount = Initializer::Get()._max_entity_amount;
        _available.assign(_max_entities_amount, true);
        for (size_t i = 0; i < _max_entities_amount; ++i) {
            _available_entities.push(i);
        }
        _entities_amount = 0;
        Logger::Info("ECS", "EntityManager", "EntityManager created with max entities amount (" + std::to_string(_max_entities_amount) + ")");
    }
    void Terminate() {
        for (size_t i = 0; i < _max_entities_amount; ++i) {
            if (!_available[i]) {
                DestroyEntity(i);
            }
        }
        _available.clear();
        Logger::Info("ECS", "EntityManager", "EntityManager terminated.");
    }
};
}  // namespace DE