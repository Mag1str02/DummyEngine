#pragma once

#include <cinttypes>
#include "entity_manager.hpp"
#include "../Component/component_manager.hpp"

namespace DE
{
    using EntityId = int64_t;

    class Entity
    {
    private:
        friend class EntityManager;
        EntityId _id;

    public:
        Entity()
        {
            _id = EntityManager::Get().CreateEntity();
        }
        ~Entity(){
            EntityManager::Get().DestroyEntity(_id);
        }

        void Destroy()
        {
            EntityManager::Get().DestroyEntity(_id);
            _id = -1;
        }
        void Create(){
            EntityManager::Get().DestroyEntity(_id);
            _id = EntityManager::Get().CreateEntity();
        }
        bool valid()
        {
            return _id != -1;
        }
    };
} // namespace DE
