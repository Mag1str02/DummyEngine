#pragma once

#include "Addition/Base.h"
#include "ToolBox/Dev/Logger.h"
#include "Core/ECS/ComponentArray.hpp"

namespace DE
{
    using ComponentId = int16_t;
    class ComponentManager
    {
    private:
        friend class Entity;
        friend class Initializer;

        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> m_ComponentArrays;
        std::unordered_map<std::string, ComponentId> m_ComponentId;
        size_t m_EntityAmount;

        static std::string NormalTypeName(const std::string& s_name)
        {
            for (size_t i = 1; i < s_name.size(); ++i)
            {
                if ('0' > s_name[i] || s_name[i] > '9')
                {
                    return s_name.substr(i, s_name.size() - i);
                }
            }
            return s_name;
        }

        ComponentManager() {}

        template <typename ComponentType>
        void RegisterComponent()
        {
            const char* component_name = typeid(ComponentType).name();

            m_ComponentId[component_name] = m_ComponentId.size();
            m_ComponentArrays[component_name] =
                std::make_shared<ComponentArray<ComponentType>>(ComponentArray<ComponentType>(m_EntityAmount));

            Logger::Info("ECS", "ComponentManager", "Registered Component (" + NormalTypeName(component_name) + ")");
        }

        template <typename ComponentType>
        void AddComponent(EntityId entity_id, ComponentType component = ComponentType())
        {
            GetComponentArray<ComponentType>()->InsertComponent(entity_id, component);
        }
        template <typename ComponentType>
        void RemoveComponent(EntityId entity_id)
        {
            GetComponentArray<ComponentType>()->RemoveComponent(entity_id);
        }
        void EntityDestroyed(EntityId entity_id)
        {
            for (auto& [name, component_array] : m_ComponentArrays)
            {
                component_array->EntityDestroyed(entity_id);
            }
        }

        template <typename ComponentType>
        ComponentType& GetComponent(EntityId entity_id)
        {
            return (*GetComponentArray<ComponentType>())[entity_id];
        }
        template <typename ComponentType>
        ComponentId GetComponentId(ComponentType component = ComponentType())
        {
            if (m_ComponentId.find(typeid(ComponentType).name()) != m_ComponentId.end())
            {
                return m_ComponentId[typeid(ComponentType).name()];
            }
            return -1;
        }

        void Initialize() {}
        void Terminate()
        {
            m_ComponentArrays.clear();
            m_ComponentId.clear();
        }

        void ExtendArrays()
        {
            ++m_EntityAmount;
            for (auto& [name, component_array] : m_ComponentArrays)
            {
                component_array->ExtendArray();
            }
        }

    public:
        static ComponentManager& Get()
        {
            static ComponentManager component_manager;
            return component_manager;
        }

        template <typename ComponentType>
        std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray()
        {
            if (m_ComponentArrays.find(typeid(ComponentType).name()) == m_ComponentArrays.end())
            {
                RegisterComponent<ComponentType>();
            }
            return std::static_pointer_cast<ComponentArray<ComponentType>>(
                m_ComponentArrays[typeid(ComponentType).name()]);
        }

        void LogState()
        {
            std::string log =
                "Current "
                "State.\n----------------------------------------------------------------------------------------------"
                "-\n";
            for (const auto& [name, component_array] : m_ComponentArrays)
            {
                log.append("Component type " + NormalTypeName(name) + "\n");
                log.append(component_array->LogState());
            }
            log.append(
                "-----------------------------------------------------------------------------------------------");
            Logger::Info("ECS", "ComponentManager", log);
        }
    };
}  // namespace DE