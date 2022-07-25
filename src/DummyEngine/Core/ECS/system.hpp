#pragma once

#include "Core/ECS/ComponentManager.hpp"

namespace DE
{
    class System
    {
    protected:
        template <typename ComponentType>
        ComponentArray<ComponentType>& GetComponentArray()
        {
            return *(ComponentManager::Get().GetComponentArray<ComponentType>());
        }

    public:
        virtual std::string GetName() const = 0;
        virtual ~System() = default;
        virtual void Update(double dt) = 0;
    };

}  // namespace DE