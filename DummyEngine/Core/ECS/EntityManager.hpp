#pragma once

namespace DE
{
#ifdef ECS_IMPLEMENTATION
    std::pair<U32, U32> EntityManager::CreateEntity()
    {
        if (m_AvailableEntities.empty())
        {
            m_AvailableEntities.push(m_States.size());
            m_States.push_back(false);
            m_Generations.push_back(0);
        }
        auto id = m_AvailableEntities.front();
        m_AvailableEntities.pop();
        m_States[id] = true;
        ++m_Generations[id];
        return std::make_pair(id, m_Generations[id]);
    }
    void EntityManager::Destroy(U32 id)
    {
        m_States[id] = false;
        m_AvailableEntities.push(id);
    }
    bool EntityManager::Valid(U32 id, U32 gen) const { return m_States[id] && m_Generations[id] == gen; }

    U32 EntityManager::NextEntity(U32 id) const
    {
        if (id >= m_States.size())
        {
            return m_States.size();
        }
        do
        {
            ++id;
        }
        while (id < m_States.size() && !m_States[id]);
        return id;
    }
    U32 EntityManager::Generation(U32 id) const { return m_Generations[id]; }
    U32 EntityManager::BeginEntity() const
    {
        U32 res = 0;
        while (res < m_States.size() && !m_States[res])
        {
            ++res;
        }
        return res;
    }
    U32 EntityManager::EndEntity() const { return m_States.size(); }
#endif
}  // namespace DE