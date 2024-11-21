#pragma once

namespace DummyEngine {
    template <typename ComponentType> void* ComponentArray<ComponentType>::AddComponent(U32 id, void* component) {
        DE_ASSERT(!entity_to_index_.contains(id), "Adding component which already exists to entity {}", id);
        entity_to_index_[id] = component_array_.size();
        index_to_entity_.push_back(id);
        component_array_.emplace_back(std::move(*reinterpret_cast<ComponentType*>(component)));
        return &component_array_[entity_to_index_[id]];
    }
    template <typename ComponentType> void* ComponentArray<ComponentType>::GetComponent(U32 id) {
        DE_ASSERT(entity_to_index_.contains(id), "Accessing non-existing component of entity {}", id);
        return &component_array_[entity_to_index_[id]];
    }
    template <typename ComponentType> bool ComponentArray<ComponentType>::HasComponent(U32 id) {
        return entity_to_index_.contains(id);
    }
    template <typename ComponentType> void ComponentArray<ComponentType>::RemoveComponent(U32 id) {
        DE_ASSERT(entity_to_index_.contains(id), "Removing non-existing component from entity {}", id);
        U32 index = entity_to_index_[id];
        if (index != component_array_.size() - 1) {
            component_array_[index]                   = std::move(component_array_.back());
            index_to_entity_[index]                   = index_to_entity_.back();
            entity_to_index_[index_to_entity_.back()] = index;
        }
        component_array_.pop_back();
        index_to_entity_.pop_back();
        entity_to_index_.erase(id);
    }
}  // namespace DummyEngine
