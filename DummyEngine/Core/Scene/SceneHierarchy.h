#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class SceneHierarchy {
    public:
        class Node {
        public:
            Node() = default;

            bool Valid() const;
            bool IsEntity() const;
            bool IsFolder() const;
            bool IsAnsestorOf(const Node& child) const;

            U32  GetID() const;
            Node GetParent() const;

            bool Attach(Node other);
            Node AddEntity(Entity entity);
            Node AddFolder(const std::string& name);
            void Delete();

            Entity            GetEntity() const;
            std::vector<Node> GetChilds() const;
            std::string&      GetName();

            bool operator==(const Node& other) const;
            bool operator!=(const Node& other) const;

        private:
            friend class SceneHierarchy;
            Node(SceneHierarchy* owner, U32 id);

            SceneHierarchy* m_Owner = nullptr;
            U32             m_ID    = UINT32_MAX;
        };

        SceneHierarchy(const std::string& name);

        Node GetRoot();
        Node AddFolder(const std::string& name);
        Node AddEntity(Entity entity);
        void DeleteNode(Node node);
        bool Attach(Node parent, Node child);

    private:
        bool IsAnsestor(U32 parent, U32 child) const;

        struct NodeData {
            NodeData() = default;
            NodeData(Entity entity, U32 parent = -1);
            NodeData(const std::string& name, U32 parent = -1);

            struct FolderNode {
                std::string      name;
                std::vector<U32> childs;
                FolderNode(const std::string& name);
            };
            struct EntityNode {
                Entity entity;
                EntityNode(Entity ent);
            };

            std::variant<std::monostate, FolderNode, EntityNode> node;
            U32                                                  parent;
        };
        std::vector<NodeData> m_Nodes;
        std::queue<U32>       m_AvailableNodes;
    };

}  // namespace DE