#pragma once

#include "DummyEngine/Core/ECS/ComponentArray.h"
#include "DummyEngine/Core/ECS/ComponentManager.h"
#include "DummyEngine/Core/ECS/Entity.h"
#include "DummyEngine/Core/ECS/Signature.h"
#include "DummyEngine/Core/ECS/Storage.h"

namespace DummyEngine {

#ifdef ECS_IMPLEMENTATION
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
        add_handlers_[std::type_index(typeid(ComponentType))] = func;
    }
    template <typename ComponentType> void ComponentManager::SetRemoveHandler(std::function<void(Entity)> func) {
        RegisterComponent<ComponentType>();
        remove_handlers_[std::type_index(typeid(ComponentType))] = func;
    }

    template <typename ComponentType> ComponentType* ComponentManager::AddComponent(U32 entity_id, const ComponentType& component) {
        RegisterComponent<ComponentType>();
        ValidateSignature(entity_id);
        signatures_[entity_id].Set(component_id_[std::type_index(typeid(ComponentType))], true);
        auto* c = reinterpret_cast<ComponentType*>(
            component_arrays_[std::type_index(typeid(ComponentType))]->AddComponent(entity_id, const_cast<ComponentType*>(&component)));
        add_handlers_[std::type_index(typeid(ComponentType))](storage_->GetEntity(entity_id));
        return c;
    }
    template <typename ComponentType> ComponentType* ComponentManager::GetComponent(U32 entity_id) {
        ValidateSignature(entity_id);
        if (!HasComponent<ComponentType>(entity_id)) {
            return nullptr;
        }
        return reinterpret_cast<ComponentType*>(component_arrays_[std::type_index(typeid(ComponentType))]->GetComponent(entity_id));
    }
    template <typename ComponentType> void ComponentManager::RemoveComponent(U32 entity_id) {
        ValidateSignature(entity_id);
        if (HasComponent<ComponentType>(entity_id)) {
            remove_handlers_[std::type_index(typeid(ComponentType))](storage_->GetEntity(entity_id));
            signatures_[entity_id].Set(component_id_[std::type_index(typeid(ComponentType))], false);
            component_arrays_[std::type_index(typeid(ComponentType))]->RemoveComponent(entity_id);
        }
    }
    template <typename ComponentType> bool ComponentManager::HasComponent(U32 entity_id) const {
        if (signatures_.size() < entity_id + 1 || component_id_.find(std::type_index(typeid(ComponentType))) == component_id_.end()) {
            return false;
        }

        return signatures_.at(entity_id).Get(component_id_.at(std::type_index(typeid(ComponentType))));
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
        if (component_id_.find(std::type_index(typeid(T))) == component_id_.end()) {
            return false;
        }
        return ValidateComponents<Components...>();
    }

    template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, Signature>::type ComponentManager::GetSignature() const {
        return Signature();
    }
    template <typename T, typename... Components> Signature ComponentManager::GetSignature() const {
        Signature res = GetSignature<Components...>();
        res.Set(component_id_.at(std::type_index(typeid(T))), true);
        return res;
    }

    template <typename ComponentType> void ComponentManager::RegisterComponent() {
        if (component_id_.find(std::type_index(typeid(ComponentType))) == component_id_.end()) {
            auto default_handler                                  = [](Entity) {};
            component_id_[std::type_index(typeid(ComponentType))] = component_id_.size();
            component_arrays_[std::type_index(typeid(ComponentType))] =
                std::make_shared<ComponentArray<ComponentType>>(ComponentArray<ComponentType>());
            add_handlers_[std::type_index(typeid(ComponentType))]    = default_handler;
            remove_handlers_[std::type_index(typeid(ComponentType))] = default_handler;
        }
    }
}  // namespace DummyEngine