#pragma once

namespace DE
{
#ifdef ECS_IMPLEMENTATION
    std::pair<uint32_t, uint32_t> EntityManager::CreateEntity()
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
    void EntityManager::Destroy(uint32_t id)
    {
        m_States[id] = false;
        m_AvailableEntities.push(id);
    }
    bool EntityManager::Valid(uint32_t id, uint32_t gen) const { return m_States[id] && m_Generations[id] == gen; }

    uint32_t EntityManager::NextEntity(uint32_t id) const
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
    uint32_t EntityManager::Generation(uint32_t id) const { return m_Generations[id]; }
    uint32_t EntityManager::BeginEntity() const
    {
        uint32_t res = 0;
        while (res < m_States.size() && !m_States[res])
        {
            ++res;
        }
        return res;
    }
    uint32_t EntityManager::EndEntity() const { return m_States.size(); }
#endif
}  // namespace DE