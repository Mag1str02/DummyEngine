#pragma once

#include "Forward.h"

namespace DummyEngine {

    class Entity {
    public:
        Entity();
        ~Entity()                        = default;
        Entity(const Entity&)            = default;
        Entity(Entity&&)                 = default;
        Entity& operator=(const Entity&) = default;
        Entity& operator=(Entity&&)      = default;

        bool Valid() const;
        void Destroy();

        template <typename ComponentType> ComponentType* AddComponent(ComponentType component = ComponentType());
        template <typename ComponentType> ComponentType* GetComponent();
        template <typename ComponentType> ComponentType& Add(ComponentType component = ComponentType());
        template <typename ComponentType> ComponentType& Get();
        template <typename ComponentType> bool           Has() const;
        template <typename ComponentType> void           Remove();

        bool operator==(const Entity& other) const;
        bool operator!=(const Entity& other) const;

    private:
        template <typename... Components> friend class StorageView;
        friend struct std::hash<Entity>;
        friend class Storage;

        U32              id_;
        U32              gen_;
        WeakRef<Storage> storage_;
    };

}  // namespace DummyEngine