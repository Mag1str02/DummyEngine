#pragma once

#include "Addition/Base.h"
#include "Core/ECS/Entity.hpp"

namespace DE
{

    class SceneHierarchyNode
    {
    public:
        SceneHierarchyNode(const std::string& name = "", Entity entity = Entity()) : m_Entity(entity), m_Name(name), m_Parent(nullptr) {}

        void AttachChild(Ref<SceneHierarchyNode> child)
        {
            m_Childs.push_back(child);
            child->m_Parent = this;
        }
        void DetachChild(Ref<SceneHierarchyNode> child)
        {
            child->m_Parent = nullptr;
            std::erase(m_Childs, child);
        }
        SceneHierarchyNode* GetParent() const { return m_Parent; }

        const std::string& GetName() const { return m_Name; }
        void               SetName(const std::string& name) { m_Name = name; }
        void               SetEntity(const Entity& entity) { m_Entity = entity; }
        bool               IsEntity() const { return m_Entity.Valid(); }

    private:
        std::vector<Ref<SceneHierarchyNode>> m_Childs;
        Entity                               m_Entity;
        std::string                          m_Name;
        SceneHierarchyNode*                  m_Parent;
    };

}  // namespace DE