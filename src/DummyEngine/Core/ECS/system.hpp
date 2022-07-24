#pragma once

#include "DummyEngine/Core/ECS/component_manager.hpp"

namespace DE {
class System {
protected:
    template <typename ComponentType>
    ComponentArray<ComponentType>& GetComponentArray() {
        return *(ComponentManager::Get().GetComponentArray<ComponentType>());
    }

public:
    virtual ~System() = default;
    virtual void Update(double dt) = 0;
};

}  // namespace DE