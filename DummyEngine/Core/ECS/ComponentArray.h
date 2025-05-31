#pragma once

#include "Forward.h"

namespace DummyEngine {

    class IComponentArray {
    public:
        virtual ~IComponentArray()                          = default;
        virtual void* AddComponent(U32 id, void* component) = 0;
        virtual void* GetComponent(U32 id)                  = 0;
        virtual bool  HasComponent(U32 id)                  = 0;
        virtual void  RemoveComponent(U32 id)               = 0;

        virtual const std::vector<U32>& GetEntities() const = 0;
    };
    template <typename ComponentType> class ComponentArray : public IComponentArray {
    public:
        ComponentArray() = default;
        virtual void* AddComponent(U32 id, void* component) override;
        virtual void* GetComponent(U32 id) override;
        virtual bool  HasComponent(U32 id) override;
        virtual void  RemoveComponent(U32 id) override;

        virtual const std::vector<U32>& GetEntities() const override;

    private:
        std::unordered_map<U32, U32> entity_to_index_;
        std::vector<U32>             index_to_entity_;
        std::vector<ComponentType>   component_array_;
    };

}  // namespace DummyEngine