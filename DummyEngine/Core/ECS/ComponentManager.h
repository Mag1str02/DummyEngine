#pragma once

#include "Forward.h"

namespace DummyEngine {

    class ComponentManager {
    public:
        explicit ComponentManager(Storage* storage);

        template <typename ComponentType> ComponentType* AddComponent(U32 entity_id, const ComponentType& component);
        template <typename ComponentType> ComponentType* GetComponent(U32 entity_id);
        template <typename ComponentType> void           RemoveComponent(U32 entity_id);
        template <typename ComponentType> bool           HasComponent(U32 entity_id) const;
        void                                             Destroy(U32 entity_id);

        template <typename ComponentType> void SetAddHandler(std::function<void(Entity)> func);
        template <typename ComponentType> void SetRemoveHandler(std::function<void(Entity)> func);

        template <typename... Components> Signature BuildSignature();
        bool                                        Matches(U32 id, const Signature& signature) const;

    private:
        template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, bool>::type ValidateComponents() const;
        template <typename T, typename... Components> bool                                                ValidateComponents() const;

        template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, Signature>::type GetSignature() const;
        template <typename T, typename... Components> Signature                                                GetSignature() const;

        void                                   ValidateSignature(U32 entity_id);
        template <typename ComponentType> void RegisterComponent();

        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> component_arrays_;
        std::unordered_map<std::type_index, U32>                              component_id_;
        std::unordered_map<std::type_index, std::function<void(Entity)>>      add_handlers_;
        std::unordered_map<std::type_index, std::function<void(Entity)>>      remove_handlers_;
        std::vector<Signature>                                                signatures_;
        Storage*                                                              storage_;
    };

}  // namespace DummyEngine