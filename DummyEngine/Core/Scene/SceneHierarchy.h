#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    // class SceneHierarchy {
    // public:
    //     SceneHierarchy(const std::string& name) { m_Nodes.emplace_back(name); }
    //     class Node {
    //     public:
    //         Node(Entity entity) : m_Node({.entity = entity}) {}
    //         Node(const std::string& name) : m_Node({.name = name, .childs = {}}) {}
    //         bool IsEntity() const {}

    //     private:
    //         struct FolderNode {
    //             std::string      name;
    //             std::vector<U32> childs;
    //         };
    //         struct EntityNode {
    //             Entity entity;
    //         };
    //         std::variant<FolderNode, EntityNode> m_Node;
    //         U32                                  m_Parent = -1;
    //     };

    // private:
    //     std::vector<Node> m_Nodes;
    // };
    class SceneHierarchyNode {
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
        std::string                          m_Name;
        Entity                               m_Entity;
        std::vector<Ref<SceneHierarchyNode>> m_Childs;
        SceneHierarchyNode*                  m_Parent;
        bool                                 m_Selected;
    };

}  // namespace DE