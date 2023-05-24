#pragma once

namespace DE {
#ifdef ECS_IMPLEMENTATION
    Entity::Entity() : m_ID(0), m_Gen(0) {}

    bool Entity::Valid() const {
        auto storage = m_Storage.lock();
        return storage && storage->Valid(m_ID, m_Gen);
    }
    void Entity::Destroy() {
        auto storage = m_Storage.lock();
        if (storage) {
            storage->Destroy(m_ID, m_Gen);
        }
        *this = Entity();
    }
    bool Entity::operator==(const Entity& other) const {
        return m_ID == other.m_ID && m_Gen == other.m_Gen && m_Storage.lock() == other.m_Storage.lock();
    }
    bool Entity::operator!=(const Entity& other) const {
        return m_ID != other.m_ID || m_Gen != other.m_Gen || m_Storage.lock() != other.m_Storage.lock();
    }
#endif

    template <typename ComponentType> ComponentType* Entity::AddComponent(ComponentType component) {
        auto storage = m_Storage.lock();
        return (storage ? storage->AddComponent<ComponentType>(m_ID, m_Gen, component) : nullptr);
    }
    template <typename ComponentType> ComponentType* Entity::GetComponent() {
        auto storage = m_Storage.lock();
        return (storage ? storage->GetComponent<ComponentType>(m_ID, m_Gen) : nullptr);
    }
    template <typename ComponentType> ComponentType& Entity::Add(ComponentType component) {
        auto storage = m_Storage.lock();
        DE_ASSERT(storage, "Adding component to entity with destructed storage");
        auto ptr = storage->AddComponent<ComponentType>(m_ID, m_Gen, component);
//        DE_ASSERT(ptr, "Failed to add (", DemangledName<ComponentType>(), ") to entity (", m_ID, ")");
        return *ptr;
    }
    template <typename ComponentType> ComponentType& Entity::Get() {
        auto storage = m_Storage.lock();
        auto ptr     = storage->GetComponent<ComponentType>(m_ID, m_Gen);
//        DE_ASSERT(ptr, "Failed to get (", DemangledName<ComponentType>(), ") of entity (", m_ID, ")");
        return *ptr;
    }
    template <typename ComponentType> bool Entity::Has() const {
        auto storage = m_Storage.lock();
        if (!storage) {
            return false;
        }
        return storage->HasComponent<ComponentType>(m_ID, m_Gen);
    }
    template <typename ComponentType> void Entity::Remove() {
        auto storage = m_Storage.lock();
        if (storage) {
            storage->RemoveComponent<ComponentType>(m_ID, m_Gen);
        }
    }

}  // namespace DE

namespace std {
    template <> struct hash<DE::Entity> {
        std::size_t operator()(const DE::Entity& entity) const {
            return hash<U64>()(((U64)entity.m_ID << 32) + entity.m_Gen) ^ hash<U64>()(reinterpret_cast<U64>(entity.m_Storage.lock().get()));
        }
    };
}  // namespace std