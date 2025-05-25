#pragma once

#include "Storage.hpp"  // IWYU pragma: keep

#include "DummyEngine/Core/ECS/ComponentArray.h"
#include "DummyEngine/Core/ECS/ComponentManager.h"
#include "DummyEngine/Core/ECS/Entity.h"
#include "DummyEngine/Core/ECS/StorageView.h"

namespace DummyEngine {

    template <typename... Components> StorageView<Components...>::StorageView(Storage* storage) {
        const std::vector<U32>* min_entities        = nullptr;
        auto                    update_min_entities = [&min_entities](const IComponentArray* array) {
            if (array == nullptr) {
                return;
            }
            const auto* entities = &array->GetEntities();
            if (min_entities == nullptr || min_entities->size() > entities->size()) {
                min_entities = entities;
            }
        };
        ((update_min_entities(storage->component_manager_.GetComponentArray<Components>())), ...);
        auto signature = storage->component_manager_.BuildSignature<Components...>();
        if (min_entities == nullptr) {
            return;
        }
        for (const auto& entity : *min_entities) {
            if (!storage->component_manager_.Matches(entity, signature)) {
                continue;
            }
            Entity res;
            res.id_      = entity;
            res.gen_     = storage->entity_manager_.Generation(entity);
            res.storage_ = storage->weak_from_this();
            DE_ASSERT(res.Valid(), "Entity with id {} and generation {} is invalid", res.id_, res.gen_);
            view_.emplace_back(std::move(res));
        }
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::begin() {
        return view_.begin();
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::end() {
        return view_.end();
    }
    template <typename... Components> bool StorageView<Components...>::Empty() {
        return begin() == end();
    }

}  // namespace DummyEngine