#pragma once

#include <queue>

#include "../../Initializer/initializer.h"

namespace DE
{

    using EntityId = int64_t;

    class EntityManager
    {
    private:
        std::queue<EntityId> _available_entities;
        std::vector<bool> _available;
        uint64_t _entities_amount;

        uint64_t _max_entities_amount;

        EntityManager()
        {
            _max_entities_amount = Initializer::Get()._max_entity_amount;
            _available.assign(_max_entities_amount, true);
            for (size_t i = 0; i < _max_entities_amount; ++i)
            {
                _available_entities.push(i);
            }
            _entities_amount = 0;
        }

    public:
        static EntityManager &Get()
        {
            static EntityManager entity_manager;
            return entity_manager;
        }

        EntityId CreateEntity()
        {
            if (_available.empty())
            {
                return -1;
            }
            EntityId new_entity = _available_entities.front();
            _available_entities.pop();
            _available[new_entity] = false;
            ++_entities_amount;
            return new_entity;
        }
        EntityId GetEntity(EntityId id)
        {
            if (!_available[id])
            {
                return id;
            }
            return -1;
        }
        void DestroyEntity(EntityId entity)
        {
            if (entity < 0 || entity >= _max_entities_amount || _available[entity])
            {
                return;
            }
            _available[entity] = true;
            --_entities_amount;
            _available_entities.push(entity);
        }
    };

    EntityId deCreateEntity()
    {
        return EntityManager::Get().CreateEntity();
    }
    EntityId deGetEntity(EntityId entity)
    {
        return EntityManager::Get().GetEntity(entity);
    }
    void deDestroyEntity(EntityId entity)
    {
        EntityManager::Get().DestroyEntity(entity);
    }

} // namespace DE