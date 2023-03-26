#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ECS/Entity.hpp"

namespace DE
{

    class SceneHierarchyNode
    {
    public:
        SceneHierarchyNode(const std::string& name = "", Entity entity = Entity());

        void                    AttachChild(Ref<SceneHierarchyNode> child);
        Ref<SceneHierarchyNode> DetachChild(SceneHierarchyNode* child);
        Ref<SceneHierarchyNode> Detach();

        SceneHierarchyNode* GetParent() const;
        const std::string&  GetName();
        Entity              GetEntity() const;
        void                SetName(const std::string& name);
        void                SetEntity(const Entity& entity);
        bool                IsEntity() const;
        bool&               Selected();
        bool                IsAnsestor(SceneHierarchyNode* node) const;

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