#pragma once

namespace DE {
#ifdef ECS_IMPLEMENTATION
    bool Signature::Get(U64 id) const {
        return (m_Data.size() <= (id >> 6) ? false : (m_Data[id >> 6] >> (id & 63)) & 1);
    }
    void Signature::Set(U64 id, bool value) {
        if (m_Data.size() <= id >> 6) {
            m_Data.resize((id >> 6) + 1);
        }
        m_Data[id >> 6] &= ~(1 << (id & 63));
        m_Data[id >> 6] |= ((U64)value << (id & 63));
    }
    bool Signature::Matches(const Signature& required) const {
        size_t i;
        size_t mn = std::min(required.m_Data.size(), m_Data.size());
        for (i = 0; i < mn; ++i) {
            if ((m_Data[i] & required.m_Data[i]) != required.m_Data[i]) {
                return false;
            }
        }
        while (i < required.m_Data.size()) {
            if (required.m_Data[i++]) {
                return false;
            }
        }
        return true;
    }

    ComponentManager::ComponentManager(Storage* storage) : m_Storage(storage) {}
    void ComponentManager::Destroy(U32 entity_id) {
        Entity e = m_Storage->GetEntity(entity_id);
        for (const auto& [id, array] : m_ComponentArrays) {
            if (array->HasComponent(entity_id)) {
                m_RemoveHandlers[id](e);
                array->RemoveComponent(entity_id);
            }
        }
        m_Signatures[entity_id] = Signature();
    }
    bool ComponentManager::Matches(U32 id, const Signature& signature) const {
        if (m_Signatures.size() <= id) {
            return false;
        }
        return m_Signatures.at(id).Matches(signature);
    }
    void ComponentManager::ValidateSignature(U32 entity_id) {
        while (m_Signatures.size() < entity_id + 1) {
            m_Signatures.push_back(Signature());
        }
    }

#endif

    template <typename ComponentType> void ComponentManager::SetAddHandler(std::function<void(Entity)> func) {
        RegisterComponent<ComponentType>();
        std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(ComponentType)));
        m_AddHandlers[INDEX(ComponentType)] = func;
    }
    template <typename ComponentType> void ComponentManager::SetRemoveHandler(std::function<void(Entity)> func) {
        RegisterComponent<ComponentType>();
        std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(ComponentType)));
        m_RemoveHandlers[INDEX(ComponentType)] = func;
    }

    template <typename ComponentType> ComponentType* ComponentManager::AddComponent(U32 entity_id, const ComponentType& component) {
        RegisterComponent<ComponentType>();
        ValidateSignature(entity_id);
        m_ComponentMutex.get(INDEX(ComponentType)).lock();
        m_Signatures[entity_id].Set(m_ComponentId[INDEX(ComponentType)], true);
        auto* c = reinterpret_cast<ComponentType*>(
            m_ComponentArrays[INDEX(ComponentType)]->AddComponent(entity_id, const_cast<ComponentType*>(&component)));
        auto& addHandler = m_AddHandlers[INDEX(ComponentType)];
        m_ComponentMutex.get(INDEX(ComponentType)).unlock();
        addHandler(m_Storage->GetEntity(entity_id));
        return c;
    }
    template <typename ComponentType> ComponentType* ComponentManager::GetComponent(U32 entity_id) {
        ValidateSignature(entity_id);
        if (!HasComponent<ComponentType>(entity_id)) {
            return nullptr;
        }
        std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(ComponentType)));
        return reinterpret_cast<ComponentType*>(m_ComponentArrays[INDEX(ComponentType)]->GetComponent(entity_id));
    }
    template <typename ComponentType> void ComponentManager::RemoveComponent(U32 entity_id) {
        ValidateSignature(entity_id);
        if (HasComponent<ComponentType>(entity_id)) {
            std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(ComponentType)));
            m_RemoveHandlers[INDEX(ComponentType)](m_Storage->GetEntity(entity_id));
            m_Signatures[entity_id].Set(m_ComponentId[INDEX(ComponentType)], false);
            m_ComponentArrays[INDEX(ComponentType)]->RemoveComponent(entity_id);
        }
    }
    template <typename ComponentType> bool ComponentManager::HasComponent(U32 entity_id) {
        std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(ComponentType)));
        if (m_Signatures.size() < entity_id + 1 || m_ComponentId.find(INDEX(ComponentType)) == m_ComponentId.end()) {
            return false;
        }

        return m_Signatures.at(entity_id).Get(m_ComponentId.at(INDEX(ComponentType)));
    }

    template <typename... Components> Signature ComponentManager::BuildSignature() {
        if (!ValidateComponents<Components...>()) {
            Signature res;
            res.Set(m_ComponentId.size(), true);
            return res;
        }
        return GetSignature<Components...>();
    }

    template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, bool>::type ComponentManager::ValidateComponents() {
        return true;
    }
    template <typename T, typename... Components> bool ComponentManager::ValidateComponents() {
        m_ComponentMutex.get(INDEX(T)).lock();
        if (m_ComponentId.find(INDEX(T)) == m_ComponentId.end()) {
            return false;
        }
        m_ComponentMutex.get(INDEX(T)).unlock();
        return ValidateComponents<Components...>();
    }

    template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, Signature>::type ComponentManager::GetSignature() {
        return Signature();
    }
    template <typename T, typename... Components> Signature ComponentManager::GetSignature() {
        Signature res = GetSignature<Components...>();
        std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(T)));
        res.Set(m_ComponentId.at(INDEX(T)), true);
        return res;
    }

    template <typename ComponentType> void ComponentManager::RegisterComponent() {
        std::lock_guard<std::mutex> lock(m_ComponentMutex.get(INDEX(ComponentType)));
        if (!m_ComponentId.contains(INDEX(ComponentType))) {
            auto default_handler                    = [](Entity e) {};
            m_ComponentId[INDEX(ComponentType)]     = m_ComponentId.size();
            m_ComponentArrays[INDEX(ComponentType)] = std::make_shared<ComponentArray<ComponentType>>(ComponentArray<ComponentType>());
            m_AddHandlers[INDEX(ComponentType)]     = default_handler;
            m_RemoveHandlers[INDEX(ComponentType)]  = default_handler;
        }
    }
}  // namespace DE