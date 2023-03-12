#pragma once

namespace DE
{
    template <typename ComponentType> void* ComponentArray<ComponentType>::AddComponent(uint32_t id, void* component)
    {
        if (m_ComponentArray.size() < id + 1)
        {
            m_EntityToIndex[id] = m_ComponentArray.size();
            m_ComponentArray.push_back(*reinterpret_cast<ComponentType*>(component));
        }
        else
        {
            m_ComponentArray[m_EntityToIndex[id]] = *reinterpret_cast<ComponentType*>(component);
        }
        return &m_ComponentArray[m_EntityToIndex[id]];
    }
    template <typename ComponentType> void* ComponentArray<ComponentType>::GetComponent(uint32_t id)
    {
        return &m_ComponentArray[m_EntityToIndex[id]];
    }
    template <typename ComponentType> void ComponentArray<ComponentType>::RemoveComponent(uint32_t id)
    {
        if (m_EntityToIndex[id] != m_ComponentArray.size() - 1)
        {
            m_ComponentArray[m_EntityToIndex[id]] = m_ComponentArray.back();
        }
        m_ComponentArray.pop_back();
    }

}  // namespace DE