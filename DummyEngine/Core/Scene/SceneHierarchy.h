#pragma once

#include "DummyEngine/Core/ECS/ECS.h"

namespace DummyEngine {

    class SceneHierarchy {
    public:
        class Node;

        explicit SceneHierarchy(const std::string& name);

        Node GetRoot();
        Node AddFolder(const std::string& name);
        Node AddEntity(Entity entity);
        void DeleteNode(Node node);
        bool Attach(Node parent, Node child);

    private:
        bool IsAnsestor(U32 parent, U32 child) const;

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

            SceneHierarchy* owner_ = nullptr;
            U32             id_    = UINT32_MAX;
        };

    private:
        struct NodeData {
        public:
            NodeData() = default;
            explicit NodeData(Entity entity, U32 parent = -1);
            explicit NodeData(const std::string& name, U32 parent = -1);

        public:
            struct FolderNode {
                std::string      Name;
                std::vector<U32> Childs;

                explicit FolderNode(const std::string& name);
            };
            struct EntityNode {
                Entity Entity;

                explicit EntityNode(class Entity entity);
            };

        public:
            std::variant<std::monostate, FolderNode, EntityNode> Node;
            U32                                                  Parent;
        };

    private:
        std::vector<NodeData> nodes_;
        std::queue<U32>       available_nodes_;
    };

}  // namespace DummyEngine