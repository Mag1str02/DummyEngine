#pragma once

namespace DE {
    template <typename ComponentType> void* ComponentArray<ComponentType>::AddComponent(U32 id, void* component) {
        DE_ASSERT(!m_EntityToIndex.contains(id), "Adding component which already exists to entity (", id, ")");
        m_EntityToIndex[id] = m_ComponentArray.size();
        m_IndexToEntity.push_back(id);
        m_ComponentArray.emplace_back(std::move(*reinterpret_cast<ComponentType*>(component)));
        return &m_ComponentArray[m_EntityToIndex[id]];
    }
    template <typename ComponentType> void* ComponentArray<ComponentType>::GetComponent(U32 id) {
        DE_ASSERT(m_EntityToIndex.contains(id), "Accessing non-existing component of entity (", id, ")");
        return &m_ComponentArray[m_EntityToIndex[id]];
    }
    template <typename ComponentType> bool ComponentArray<ComponentType>::HasComponent(U32 id) {
        return m_EntityToIndex.contains(id);
    }
    template <typename ComponentType> void ComponentArray<ComponentType>::RemoveComponent(U32 id) {
        DE_ASSERT(m_EntityToIndex.contains(id), "Removing non-existing component from entity (", id, ")");
        U32 index = m_EntityToIndex[id];
        if (index != m_ComponentArray.size() - 1) {
            m_ComponentArray[index]                 = std::move(m_ComponentArray.back());
            m_IndexToEntity[index]                  = m_IndexToEntity.back();
            m_EntityToIndex[m_IndexToEntity.back()] = index;
        }
        m_ComponentArray.pop_back();
        m_IndexToEntity.pop_back();
        m_EntityToIndex.erase(id);
    }
}  // namespace DE
