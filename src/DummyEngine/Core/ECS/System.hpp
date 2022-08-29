#pragma once

#include "Core/ECS/ComponentManager.hpp"

namespace DE
{
    class SystemManager;

    class System
    {
    private:
        ComponentManager* m_ComponentManager;

    protected:
        template <typename ComponentType>
        ComponentArray<ComponentType>& GetComponentArray()
        {
            return *(m_ComponentManager->GetComponentArray<ComponentType>());
        }

    public:
        virtual std::string GetName() const = 0;
        virtual ~System() = default;
        virtual void Update(double dt) = 0;

        //TODO: Try to hide binding mechanism in private scope.
        void Bind(ComponentManager* component_manager)
        {
            m_ComponentManager = component_manager;
        }
    };

}  // namespace DE