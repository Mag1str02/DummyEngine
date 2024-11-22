#pragma once

#include "Storage.hpp"  // IWYU pragma: keep

#include "DummyEngine/Core/ECS/Entity.h"
#include "DummyEngine/Core/ECS/StorageView.h"

namespace DummyEngine {

    template <typename... Components> StorageView<Components...>::Iterator::Iterator(U32 id, StorageView* v) : id_(id), view_(v) {}

    template <typename... Components> bool StorageView<Components...>::Iterator::operator==(const Iterator& other) const {
        return view_ == other.view_ && id_ == other.id_;
    }
    template <typename... Components> bool StorageView<Components...>::Iterator::operator!=(const Iterator& other) const {
        return view_ != other.view_ || id_ != other.id_;
    }

    template <typename... Components> StorageView<Components...>::StorageView(Storage* storage) : storage_(storage) {
        signature_ = storage_->component_manager_.BuildSignature<Components...>();
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::begin() {
        Iterator res(storage_->entity_manager_.BeginEntity(), this);
        if (!storage_->component_manager_.Matches(res.id_, signature_)) {
            ++res;
        }
        return res;
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::end() {
        return Iterator(storage_->entity_manager_.EndEntity(), this);
    }
    template <typename... Components> bool StorageView<Components...>::Empty() {
        return begin() == end();
    }

    template <typename... Components> typename StorageView<Components...>::Iterator& StorageView<Components...>::Iterator::operator++() {
        do {
            id_ = view_->storage_->entity_manager_.NextEntity(id_);
        } while (id_ != view_->storage_->entity_manager_.EndEntity() && !view_->storage_->component_manager_.Matches(id_, view_->signature_));
        return *this;
    }
    template <typename... Components> typename StorageView<Components...>::Iterator StorageView<Components...>::Iterator::operator++(int) {
        auto res = *this;
        do {
            id_ = view_->storage_->entity_manager_.NextEntity(id_);
        } while (id_ != view_->storage_->entity_manager_.EndEntity() && view_->storage_->component_manager_.Matches(id_, view_->signature_));
        return res;
    }
    template <typename... Components> Entity StorageView<Components...>::Iterator::operator*() {
        Entity res;
        res.id_      = id_;
        res.gen_     = view_->storage_->entity_manager_.Generation(id_);
        res.storage_ = view_->storage_->weak_from_this();
        return res;
    }

}  // namespace DummyEngine