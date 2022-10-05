#pragma once

#include "Addition/Base.h"
#include "Core/ECS/Entity.hpp"

namespace DE
{

    class SceneHierarchyNode
    {
    public:
        SceneHierarchyNode(const std::string& name = "", Entity entity = Entity());

        void AttachChild(Ref<SceneHierarchyNode> child);
        void DetachChild(Ref<SceneHierarchyNode> child);

        SceneHierarchyNode* GetParent() const;
        const std::string&  GetName() const;
        Entity              GetEntity() const;
        void                SetName(const std::string& name);
        void                SetEntity(const Entity& entity);
        bool                IsEntity() const;
        bool&               Selected();

        std::vector<Ref<SceneHierarchyNode>>::iterator begin();
        std::vector<Ref<SceneHierarchyNode>>::iterator end();

    private:
        std::vector<Ref<SceneHierarchyNode>> m_Childs;
        Entity                               m_Entity;
        std::string                          m_Name;
        SceneHierarchyNode*                  m_Parent;
        bool                                 m_Selected;
    };

}  // namespace DE