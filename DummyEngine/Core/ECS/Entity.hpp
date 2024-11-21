#pragma once

namespace DummyEngine {
#ifdef ECS_IMPLEMENTATION
    Entity::Entity() : id_(0), gen_(0) {}

    bool Entity::Valid() const {
        auto storage = storage_.lock();
        return storage && storage->Valid(id_, gen_);
    }
    void Entity::Destroy() {
        auto storage = storage_.lock();
        if (storage) {
            storage->Destroy(id_, gen_);
        }
        *this = Entity();
    }
    bool Entity::operator==(const Entity& other) const {
        return id_ == other.id_ && gen_ == other.gen_ && storage_.lock() == other.storage_.lock();
    }
    bool Entity::operator!=(const Entity& other) const {
        return id_ != other.id_ || gen_ != other.gen_ || storage_.lock() != other.storage_.lock();
    }
#endif

    template <typename ComponentType> ComponentType* Entity::AddComponent(ComponentType component) {
        auto storage = storage_.lock();
        return (storage ? storage->AddComponent<ComponentType>(id_, gen_, component) : nullptr);
    }
    template <typename ComponentType> ComponentType* Entity::GetComponent() {
        auto storage = storage_.lock();
        return (storage ? storage->GetComponent<ComponentType>(id_, gen_) : nullptr);
    }
    template <typename ComponentType> ComponentType& Entity::Add(ComponentType component) {
        auto storage = storage_.lock();
        DE_ASSERT(storage, "Adding component to entity with destructed storage");
        auto ptr = storage->AddComponent<ComponentType>(id_, gen_, component);
        //        DE_ASSERT(ptr, "Failed to add (", DemangledName<ComponentType>(), ") to entity (", id_, ")");
        return *ptr;
    }
    template <typename ComponentType> ComponentType& Entity::Get() {
        auto storage = storage_.lock();
        auto ptr     = storage->GetComponent<ComponentType>(id_, gen_);
        //        DE_ASSERT(ptr, "Failed to get (", DemangledName<ComponentType>(), ") of entity (", id_, ")");
        return *ptr;
    }
    template <typename ComponentType> bool Entity::Has() const {
        auto storage = storage_.lock();
        if (!storage) {
            return false;
        }
        return storage->HasComponent<ComponentType>(id_, gen_);
    }
    template <typename ComponentType> void Entity::Remove() {
        auto storage = storage_.lock();
        if (storage) {
            storage->RemoveComponent<ComponentType>(id_, gen_);
        }
    }

}  // namespace DummyEngine

namespace std {
    template <> struct hash<DummyEngine::Entity> {
        std::size_t operator()(const DummyEngine::Entity& entity) const {
            return hash<U64>()(((U64)entity.id_ << 32) + entity.gen_) ^ hash<U64>()(reinterpret_cast<U64>(entity.storage_.lock().get()));
        }
    };
}  // namespace std