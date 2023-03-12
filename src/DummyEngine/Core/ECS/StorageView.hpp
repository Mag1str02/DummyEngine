#pragma once

namespace DE
{

    template <typename... Components> StorageView<Components...>::Iterator::Iterator(uint32_t id, StorageView* v) : m_ID(id), m_View(v) {}

    template <typename... Components> bool StorageView<Components...>::Iterator::operator==(const Iterator& other) const
    {
        return m_View == other.m_View && m_ID == other.m_ID;
    }
    template <typename... Components> bool StorageView<Components...>::Iterator::operator!=(const Iterator& other) const
    {
        return m_View != other.m_View || m_ID != other.m_ID;
    }

    template <typename... Components> StorageView<Components...>::StorageView(Storage* storage) : m_Storage(storage)
    {
        m_Signature = m_Storage->m_ComponentManager.BuildSignature<Components...>();
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::begin()
    {
        Iterator res(m_Storage->m_EntityManager.BeginEntity(), this);
        if (!m_Storage->m_ComponentManager.Matches(res.m_ID, m_Signature))
        {
            ++res;
        }
        return res;
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::end()
    {
        return Iterator(m_Storage->m_EntityManager.EndEntity(), this);
    }
    template <typename... Components> bool StorageView<Components...>::Empty() { return begin() == end(); }

    template <typename... Components> typename StorageView<Components...>::Iterator& StorageView<Components...>::Iterator::operator++()
    {
        do
        {
            m_ID = m_View->m_Storage->m_EntityManager.NextEntity(m_ID);
        }
        while (m_ID != m_View->m_Storage->m_EntityManager.EndEntity() && !m_View->m_Storage->m_ComponentManager.Matches(m_ID, m_View->m_Signature));
        return *this;
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::Iterator::operator++(int)
    {
        auto res = *this;
        do
        {
            m_ID = m_View->m_Storage->m_EntityManager.NextEntity(m_ID);
        }
        while (m_ID != m_View->m_Storage->m_EntityManager.EndEntity() && m_View->m_Storage->m_ComponentManager.Matches(m_ID, m_View->m_Signature));
        return res;
    }
    template <typename... Components> Entity StorageView<Components...>::Iterator::operator*()
    {
        Entity res;
        res.m_ID      = m_ID;
        res.m_Gen     = m_View->m_Storage->m_EntityManager.Generation(m_ID);
        res.m_Storage = m_View->m_Storage;
        return res;
    }

}  // namespace DE