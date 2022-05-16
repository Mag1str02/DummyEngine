#pragma once

#include "../../Addition/types.h"
#include "../Component/component_manager.hpp"

namespace DE
{
    class ISystem
    {
    protected:
        template <typename ComponentType>
        std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray()
        {
            return ComponentManager::Get().GetComponentArray<ComponentType>();
        }

    public:
        virtual ~ISystem() = default;
        virtual void Update(double dt) = 0;
    };

}