#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

namespace DummyEngine {

    class Storage : public std::enable_shared_from_this<Storage> {
    public:
        Storage();
        // If any of remove callbacks use functions of passed entity, Destruct should be called before destructor
        void Destruct();
        ~Storage()                         = default;
        Storage(const Storage&)            = delete;
        Storage(Storage&&)                 = delete;
        Storage& operator=(const Storage&) = delete;
        Storage& operator=(Storage&&)      = delete;

        Entity                                                       CreateEntity();
        template <typename... Components> StorageView<Components...> View();

        template <typename ComponentType> void SetAddHandler(std::function<void(Entity)> func);
        template <typename ComponentType> void SetRemoveHandler(std::function<void(Entity)> func);

        template <typename SystemType> void AttachSystem(std::shared_ptr<System> system);
        void                                UpdateSystems(float dt);

    private:
        template <typename... Components> friend class StorageView;
        friend class Entity;
        friend class ComponentManager;
        friend struct StorageDeleter;

        Entity GetEntity(U32 id);
        bool   Valid(U32 id, U32 gen) const;
        void   Destroy(U32 id, U32 gen);

        template <typename ComponentType> ComponentType* AddComponent(U32 id, U32 gen, const ComponentType& component);
        template <typename ComponentType> ComponentType* GetComponent(U32 id, U32 gen);
        template <typename ComponentType> bool           HasComponent(U32 id, U32 gen) const;
        template <typename ComponentType> void           RemoveComponent(U32 id, U32 gen);

        EntityManager    entity_manager_;
        ComponentManager component_manager_;
        SystemManager    system_manager_;
    };

}  // namespace DummyEngine