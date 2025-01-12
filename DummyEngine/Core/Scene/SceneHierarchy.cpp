#include "SceneHierarchy.h"

namespace DummyEngine {

    bool SceneHierarchy::Node::Valid() const {
        return id_ != UINT32_MAX && owner_ != nullptr && !std::holds_alternative<std::monostate>(owner_->nodes_[id_].Node);
    }
    bool SceneHierarchy::Node::IsEntity() const {
        return id_ != UINT32_MAX && owner_ != nullptr && std::holds_alternative<NodeData::EntityNode>(owner_->nodes_[id_].Node);
    }
    bool SceneHierarchy::Node::IsFolder() const {
        return id_ != UINT32_MAX && owner_ != nullptr && std::holds_alternative<NodeData::FolderNode>(owner_->nodes_[id_].Node);
    }
    bool SceneHierarchy::Node::IsAnsestorOf(const Node& child) const {
        if (!Valid() || !child.Valid() || owner_ != child.owner_) {
            return false;
        }
        return owner_->IsAnsestor(id_, child.id_);
    }
    U32 SceneHierarchy::Node::GetID() const {
        return id_;
    }
    SceneHierarchy::Node SceneHierarchy::Node::GetParent() const {
        DE_ASSERT(id_ == UINT32_MAX && owner_ != nullptr, "Use of GetParent on invalid SceneHierarchy::Node");
        return Node(owner_, owner_->nodes_[id_].Parent);
    }

    bool SceneHierarchy::Node::Attach(Node other) {
        DE_ASSERT(id_ != UINT32_MAX && owner_ != nullptr, "Use of Attach on invalid SceneHierarchy::Node");
        return owner_->Attach(*this, other);
    }
    SceneHierarchy::Node SceneHierarchy::Node::AddEntity(Entity entity) {
        DE_ASSERT(IsFolder(), "Use of AddEntity on invalid SceneHierarchy::Node");
        auto ent = owner_->AddEntity(entity);
        owner_->Attach(*this, ent);
        return ent;
    }
    SceneHierarchy::Node SceneHierarchy::Node::AddFolder(const std::string& name) {
        DE_ASSERT(IsFolder(), "Use of AddFolder on invalid SceneHierarchy::Node");
        auto folder = owner_->AddFolder(name);
        owner_->Attach(*this, folder);
        return folder;
    }
    void SceneHierarchy::Node::Delete() {
        DE_ASSERT(id_ != UINT32_MAX && owner_, "Use of Delete on invalid SceneHierarchy::Node");
        owner_->DeleteNode(*this);
        *this = Node();
    }

    Entity SceneHierarchy::Node::GetEntity() const {
        DE_ASSERT(IsEntity(), "Use of GetEntity on non entity Node");
        auto& entity = std::get<NodeData::EntityNode>(owner_->nodes_[id_].Node);
        return entity.Entity;
    }
    std::vector<SceneHierarchy::Node> SceneHierarchy::Node::GetChilds() const {
        DE_ASSERT(IsFolder(), "Use of GetChilds on non folder Node");
        std::vector<Node> res;
        auto&             folder = std::get<NodeData::FolderNode>(owner_->nodes_[id_].Node);
        for (auto child : folder.Childs) {
            res.push_back(Node(owner_, child));
        }
        return res;
    }
    std::string& SceneHierarchy::Node::GetName() {
        DE_ASSERT(IsFolder(), "Use of GetName on non folder Node");
        auto& folder = std::get<NodeData::FolderNode>(owner_->nodes_[id_].Node);
        return folder.Name;
    }
    bool SceneHierarchy::Node::operator==(const Node& other) const {
        return id_ == other.id_ && owner_ == other.owner_;
    }
    bool SceneHierarchy::Node::operator!=(const Node& other) const {
        return id_ != other.id_ || owner_ != other.owner_;
    }

    SceneHierarchy::Node::Node(SceneHierarchy* owner, U32 id) : owner_(owner), id_(id) {}

    SceneHierarchy::SceneHierarchy(const std::string& name) {
        nodes_.emplace_back(name);
    }

    SceneHierarchy::Node SceneHierarchy::GetRoot() {
        return Node(this, 0);
    }
    SceneHierarchy::Node SceneHierarchy::AddFolder(const std::string& name) {
        U32 id;
        if (available_nodes_.empty()) {
            id = nodes_.size();
            nodes_.emplace_back(name, 0);
        } else {
            id = available_nodes_.front();
            available_nodes_.pop();
            nodes_[id] = NodeData(name, 0);
        }
        std::get<NodeData::FolderNode>(nodes_[0].Node).Childs.push_back(id);
        return Node(this, id);
    }
    SceneHierarchy::Node SceneHierarchy::AddEntity(Entity entity) {
        U32 id;
        if (available_nodes_.empty()) {
            id = nodes_.size();
            nodes_.emplace_back(entity, 0);
        } else {
            id = available_nodes_.front();
            available_nodes_.pop();
            nodes_[id] = NodeData(entity, 0);
        }
        std::get<NodeData::FolderNode>(nodes_[0].Node).Childs.push_back(id);
        return Node(this, id);
    }
    void SceneHierarchy::DeleteNode(Node node) {
        DE_ASSERT(node.id_ < nodes_.size(), "Use of DeleteNode on invalid SceneHierarchy::Node");
        DE_ASSERT(node.id_ != 0, "Use of DeleteNode on root SceneHierarchy::Node");
        auto& parent = std::get<NodeData::FolderNode>(nodes_[nodes_[node.id_].Parent].Node);
        for (auto it = parent.Childs.begin(); it != parent.Childs.end(); ++it) {
            if (*it == node.id_) {
                parent.Childs.erase(it);
                break;
            }
        }
        nodes_[node.id_] = NodeData();
        available_nodes_.push(node.id_);
    }
    bool SceneHierarchy::Attach(Node parent, Node child) {
        DE_ASSERT(child.id_ < nodes_.size(), "Use of Attach on invalid child SceneHierarchy::Node");
        DE_ASSERT(parent.id_ < nodes_.size(), "Use of Attach on invalid Parent SceneHierarchy::Node");
        U32 new_parent_id = parent.id_;
        U32 old_parent_id = nodes_[child.id_].Parent;
        U32 child_id      = child.id_;
        if (new_parent_id == old_parent_id || !parent.IsFolder() || IsAnsestor(child_id, new_parent_id)) {
            return false;
        }
        auto& old_parent = std::get<NodeData::FolderNode>(nodes_[old_parent_id].Node);
        auto& new_parent = std::get<NodeData::FolderNode>(nodes_[new_parent_id].Node);
        for (auto it = old_parent.Childs.begin(); it != old_parent.Childs.end(); ++it) {
            if (*it == child.id_) {
                old_parent.Childs.erase(it);
                break;
            }
        }
        nodes_[child_id].Parent = new_parent_id;
        new_parent.Childs.push_back(child_id);
        return true;
    }

    bool SceneHierarchy::IsAnsestor(U32 parent, U32 child) const {
        do {
            if (nodes_[child].Parent == parent) {
                return true;
            }
            child = nodes_[child].Parent;
        } while (child != UINT32_MAX);
        return false;
    }

    SceneHierarchy::NodeData::NodeData(Entity entity, U32 parent) : Node(EntityNode(entity)), Parent(parent) {}
    SceneHierarchy::NodeData::NodeData(const std::string& name, U32 parent) : Node(FolderNode(name)), Parent(parent) {}
    SceneHierarchy::NodeData::EntityNode::EntityNode(class Entity ent) : Entity(ent) {}
    SceneHierarchy::NodeData::FolderNode::FolderNode(const std::string& name) : Name(name) {}

}  // namespace DummyEngine