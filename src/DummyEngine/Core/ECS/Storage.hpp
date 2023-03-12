#pragma once

namespace DE
{
#ifdef ECS_IMPLEMENTATION
    Storage::Storage() : m_SystemManager(this), m_ComponentManager(this) {}
    Entity Storage::CreateEntity()
    {
        auto [id, gen] = m_EntityManager.CreateEntity();
        Entity ent;
        ent.m_ID      = id;
        ent.m_Gen     = gen;
        ent.m_Storage = this;
        return ent;
    }
    void   Storage::UpdateSystems(float dt) { m_SystemManager.Update(dt); }
    Entity Storage::GetEntity(uint32_t id)
    {
        Entity res;
        res.m_ID      = id;
        res.m_Gen     = m_EntityManager.Generation(id);
        res.m_Storage = this;
        return res;
    }
    bool Storage::Valid(uint32_t id, uint32_t gen) const { return m_EntityManager.Valid(id, gen); }
    void Storage::Destroy(uint32_t id, uint32_t gen)
    {
        if (m_EntityManager.Valid(id, gen))
        {
            m_EntityManager.Destroy(id);
            m_ComponentManager.Destroy(id);
        }
    }
#endif
    template <typename... Components> StorageView<Components...> Storage::View() { return StorageView<Components...>(this); }

    template <typename ComponentType> void Storage::SetAddHandler(std::function<void(Entity)> func)
    {
        m_ComponentManager.SetAddHandler<ComponentType>(func);
    }
    template <typename ComponentType> void Storage::SetRemoveHandler(std::function<void(Entity)> func)
    {
        m_ComponentManager.SetRemoveHandler<ComponentType>(func);
    }

    template <typename SystemType> void Storage::RegisterSystem() { m_SystemManager.RegisterSystem<SystemType>(); }

    template <typename ComponentType> ComponentType* Storage::AddComponent(uint32_t id, uint32_t gen, const ComponentType& component)
    {
        return (m_EntityManager.Valid(id, gen) ? m_ComponentManager.AddComponent<ComponentType>(id, component) : nullptr);
    }
    template <typename ComponentType> ComponentType* Storage::GetComponent(uint32_t id, uint32_t gen)
    {
        return (m_EntityManager.Valid(id, gen) ? m_ComponentManager.GetComponent<ComponentType>(id) : nullptr);
    }
    template <typename ComponentType> bool Storage::HasComponent(uint32_t id, uint32_t gen) const
    {
        return (m_EntityManager.Valid(id, gen) ? m_ComponentManager.HasComponent<ComponentType>(id) : false);
    }
    template <typename ComponentType> void Storage::RemoveComponent(uint32_t id, uint32_t gen)
    {
        if (m_EntityManager.Valid(id, gen))
        {
            m_ComponentManager.RemoveComponent<ComponentType>(id);
        }
    }

}  // namespace DE