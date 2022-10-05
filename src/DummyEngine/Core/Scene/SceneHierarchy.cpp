#include "Core/Scene/SceneHierarchy.h"

namespace DE
{
    SceneHierarchyNode::SceneHierarchyNode(const std::string& name, Entity entity) :
        m_Entity(entity), m_Name(name), m_Parent(nullptr), m_Selected(false)
    {}

    void SceneHierarchyNode::AttachChild(Ref<SceneHierarchyNode> child)
    {
        m_Childs.push_back(child);
        child->m_Parent = this;
    }
    void SceneHierarchyNode::DetachChild(Ref<SceneHierarchyNode> child)
    {
        child->m_Parent = nullptr;
        std::erase(m_Childs, child);
    }
    SceneHierarchyNode* SceneHierarchyNode::GetParent() const { return m_Parent; }
    Entity              SceneHierarchyNode::GetEntity() const { return m_Entity; }
    const std::string&  SceneHierarchyNode::GetName() const { return m_Name; }
    void                SceneHierarchyNode::SetName(const std::string& name) { m_Name = name; }
    void                SceneHierarchyNode::SetEntity(const Entity& entity) { m_Entity = entity; }
    bool                SceneHierarchyNode::IsEntity() const { return m_Entity.Valid(); }
    bool&               SceneHierarchyNode::Selected() { return m_Selected; }

    std::vector<Ref<SceneHierarchyNode>>::iterator SceneHierarchyNode::begin() { return m_Childs.begin(); }
    std::vector<Ref<SceneHierarchyNode>>::iterator SceneHierarchyNode::end() { return m_Childs.end(); }

}  // namespace DE