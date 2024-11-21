#pragma once

namespace DummyEngine {
#ifdef ECS_IMPLEMENTATION
    bool Signature::Get(U64 id) const {
        return (data_.size() <= (id >> 6) ? false : (data_[id >> 6] >> (id & 63)) & 1);
    }
    void Signature::Set(U64 id, bool value) {
        if (data_.size() <= id >> 6) {
            data_.resize((id >> 6) + 1);
        }
        data_[id >> 6] &= ~(1 << (id & 63));
        data_[id >> 6] |= ((U64)value << (id & 63));
    }
    bool Signature::Matches(const Signature& required) const {
        size_t i;
        size_t mn = std::min(required.data_.size(), data_.size());
        for (i = 0; i < mn; ++i) {
            if ((data_[i] & required.data_[i]) != required.data_[i]) {
                return false;
            }
        }
        while (i < required.data_.size()) {
            if (required.data_[i++]) {
                return false;
            }
        }
        return true;
    }

    ComponentManager::ComponentManager(Storage* storage) : storage_(storage) {}
    void ComponentManager::Destroy(U32 entity_id) {
        Entity e = storage_->GetEntity(entity_id);
        for (const auto& [id, array] : component_arrays_) {
            if (array->HasComponent(entity_id)) {
                remove_handlers_[id](e);
                array->RemoveComponent(entity_id);
            }
        }
        signatures_[entity_id] = Signature();
    }
    bool ComponentManager::Matches(U32 id, const Signature& signature) const {
        if (signatures_.size() <= id) {
            return false;
        }
        return signatures_.at(id).Matches(signature);
    }
    void ComponentManager::ValidateSignature(U32 entity_id) {
        while (signatures_.size() < entity_id + 1) {
            signatures_.push_back(Signature());
        }
    }

#endif

    template <typename ComponentType> void ComponentManager::SetAddHandler(std::function<void(Entity)> func) {
        RegisterComponent<ComponentType>();
        add_handlers_[INDEX(ComponentType)] = func;
    }
    template <typename ComponentType> void ComponentManager::SetRemoveHandler(std::function<void(Entity)> func) {
        RegisterComponent<ComponentType>();
        remove_handlers_[INDEX(ComponentType)] = func;
    }

    template <typename ComponentType> ComponentType* ComponentManager::AddComponent(U32 entity_id, const ComponentType& component) {
        RegisterComponent<ComponentType>();
        ValidateSignature(entity_id);
        signatures_[entity_id].Set(component_id_[INDEX(ComponentType)], true);
        auto* c = reinterpret_cast<ComponentType*>(
            component_arrays_[INDEX(ComponentType)]->AddComponent(entity_id, const_cast<ComponentType*>(&component)));
        add_handlers_[INDEX(ComponentType)](storage_->GetEntity(entity_id));
        return c;
    }
    template <typename ComponentType> ComponentType* ComponentManager::GetComponent(U32 entity_id) {
        ValidateSignature(entity_id);
        if (!HasComponent<ComponentType>(entity_id)) {
            return nullptr;
        }
        return reinterpret_cast<ComponentType*>(component_arrays_[INDEX(ComponentType)]->GetComponent(entity_id));
    }
    template <typename ComponentType> void ComponentManager::RemoveComponent(U32 entity_id) {
        ValidateSignature(entity_id);
        if (HasComponent<ComponentType>(entity_id)) {
            remove_handlers_[INDEX(ComponentType)](storage_->GetEntity(entity_id));
            signatures_[entity_id].Set(component_id_[INDEX(ComponentType)], false);
            component_arrays_[INDEX(ComponentType)]->RemoveComponent(entity_id);
        }
    }
    template <typename ComponentType> bool ComponentManager::HasComponent(U32 entity_id) const {
        if (signatures_.size() < entity_id + 1 || component_id_.find(INDEX(ComponentType)) == component_id_.end()) {
            return false;
        }

        return signatures_.at(entity_id).Get(component_id_.at(INDEX(ComponentType)));
    }

    template <typename... Components> Signature ComponentManager::BuildSignature() {
        if (!ValidateComponents<Components...>()) {
            Signature res;
            res.Set(component_id_.size(), true);
            return res;
        }
        return GetSignature<Components...>();
    }

    template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, bool>::type ComponentManager::ValidateComponents() const {
        return true;
    }
    template <typename T, typename... Components> bool ComponentManager::ValidateComponents() const {
        if (component_id_.find(INDEX(T)) == component_id_.end()) {
            return false;
        }
        return ValidateComponents<Components...>();
    }

    template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, Signature>::type ComponentManager::GetSignature() const {
        return Signature();
    }
    template <typename T, typename... Components> Signature ComponentManager::GetSignature() const {
        Signature res = GetSignature<Components...>();
        res.Set(component_id_.at(INDEX(T)), true);
        return res;
    }

    template <typename ComponentType> void ComponentManager::RegisterComponent() {
        if (component_id_.find(INDEX(ComponentType)) == component_id_.end()) {
            auto default_handler                    = [](Entity) {};
            component_id_[INDEX(ComponentType)]     = component_id_.size();
            component_arrays_[INDEX(ComponentType)] = std::make_shared<ComponentArray<ComponentType>>(ComponentArray<ComponentType>());
            add_handlers_[INDEX(ComponentType)]     = default_handler;
            remove_handlers_[INDEX(ComponentType)]  = default_handler;
        }
    }
}  // namespace DummyEngine