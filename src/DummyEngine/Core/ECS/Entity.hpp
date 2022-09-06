#pragma once

#include "ToolBox/Dev/Logger.h"
#include "Core/Scene/Scene.h"

namespace DE
{

    class Entity
    {
    public:
        Entity() : m_Id(-1), m_Scene(nullptr) {}
        Entity(EntityId id, Scene* scene) : m_Id(id), m_Scene(scene) {}

        bool Valid() { return m_Scene != nullptr && m_Id != -1; }
        void Destroy()
        {
            if (Valid())
            {
                m_Scene->OnEntityDestroy(*this);
                m_Scene->m_Storage.DestroyEntity(m_Id);
                m_Scene = nullptr;
            }
        }

        template <typename ComponentType> ComponentType& AddComponent(ComponentType component = ComponentType())
        {
            DE_ASSERT(m_Scene != nullptr, "Entity either was not initialized properly, or was destroyed before using AddComponent command.");
            return m_Scene->m_Storage.AddComponent(m_Id, component);
        }
        template <typename ComponentType> void RemoveComponent()
        {
            DE_ASSERT(m_Scene != nullptr, "Entity either was not initialized properly, or was destroyed before using RemoveComponent command.");
            m_Scene->m_Storage.RemoveComponent<ComponentType>(m_Id);
        }
        template <typename ComponentType> ComponentType& GetComponent()
        {
            DE_ASSERT(m_Scene != nullptr, "Entity either was not initialized properly, or was destroyed before using GetComponent command.");
            DE_ASSERT(HasComponent<ComponentType>(), "Trying to access non-existing component.");
            return m_Scene->m_Storage.GetComponent<ComponentType>(m_Id);
        }
        template <typename ComponentType> bool HasComponent()
        {
            DE_ASSERT(m_Scene != nullptr, "Entity either was not initialized properly, or was destroyed before using HasComponent command.");
            return m_Scene->m_Storage.HasComponent<ComponentType>(m_Id);
        }

    private:
        friend class Scene;

        EntityId m_Id;
        Scene* m_Scene;
    };
}  // namespace DE
