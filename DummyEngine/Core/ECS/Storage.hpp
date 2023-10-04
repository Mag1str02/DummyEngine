#pragma once

namespace DE {
#ifdef ECS_IMPLEMENTATION

    Storage::Storage() : m_SystemManager(this), m_ComponentManager(this) {}
    void Storage::Destruct() {
        for (U32 id = m_EntityManager.BeginEntity(); id != m_EntityManager.EndEntity(); id = m_EntityManager.NextEntity(id)) {
            m_ComponentManager.Destroy(id);
            m_EntityManager.Destroy(id);
        }
    }
    Entity Storage::CreateEntity() {
        auto [id, gen] = m_EntityManager.CreateEntity();
        Entity ent;
        ent.m_ID      = id;
        ent.m_Gen     = gen;
        ent.m_Storage = weak_from_this();
        return ent;
    }
    void Storage::UpdateSystems(float dt) {
        m_SystemManager.Update(dt);
    }
    Entity Storage::GetEntity(U32 id) {
        Entity res;
        res.m_ID      = id;
        res.m_Gen     = m_EntityManager.Generation(id);
        res.m_Storage = weak_from_this();
        return res;
    }
    bool Storage::Valid(U32 id, U32 gen) const {
        return m_EntityManager.Valid(id, gen);
    }
    void Storage::Destroy(U32 id, U32 gen) {
        if (m_EntityManager.Valid(id, gen)) {
            m_ComponentManager.Destroy(id);
            m_EntityManager.Destroy(id);
        }
    }
#endif
    template <typename... Components> StorageView<Components...> Storage::View() {
        return StorageView<Components...>(this);
    }

    template <typename ComponentType> void Storage::SetAddHandler(std::function<void(Entity)> func) {
        m_ComponentManager.SetAddHandler<ComponentType>(func);
    }
    template <typename ComponentType> void Storage::SetRemoveHandler(std::function<void(Entity)> func) {
        m_ComponentManager.SetRemoveHandler<ComponentType>(func);
    }

    template <typename SystemType> void Storage::AttachSystem(std::shared_ptr<System> system) {
        m_SystemManager.AttachSystem<SystemType>(system);
    }

    template <typename ComponentType> ComponentType* Storage::AddComponent(U32 id, U32 gen, const ComponentType& component) {
        return (m_EntityManager.Valid(id, gen) ? m_ComponentManager.AddComponent<ComponentType>(id, component) : nullptr);
    }
    template <typename ComponentType> ComponentType* Storage::GetComponent(U32 id, U32 gen) {
        return (m_EntityManager.Valid(id, gen) ? m_ComponentManager.GetComponent<ComponentType>(id) : nullptr);
    }
    template <typename ComponentType> bool Storage::HasComponent(U32 id, U32 gen) {
        return (m_EntityManager.Valid(id, gen) ? m_ComponentManager.HasComponent<ComponentType>(id) : false);
    }
    template <typename ComponentType> void Storage::RemoveComponent(U32 id, U32 gen) {
        if (m_EntityManager.Valid(id, gen)) {
            m_ComponentManager.RemoveComponent<ComponentType>(id);
        }
    }

}  // namespace DE