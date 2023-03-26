#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/Components.h"

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
    Ref<SceneHierarchyNode> SceneHierarchyNode::DetachChild(SceneHierarchyNode* child)
    {
        Ref<SceneHierarchyNode> res = nullptr;
        for (auto it = m_Childs.begin(); it != m_Childs.end(); ++it)
        {
            if (it->get() == child)
            {
                res = *it;
                m_Childs.erase(it);
                break;
            }
        }
        child->m_Parent = nullptr;
        return res;
    }
    Ref<SceneHierarchyNode> SceneHierarchyNode::Detach() { return m_Parent->DetachChild(this); }

    SceneHierarchyNode* SceneHierarchyNode::GetParent() const { return m_Parent; }
    Entity              SceneHierarchyNode::GetEntity() const { return m_Entity; }
    const std::string&  SceneHierarchyNode::GetName()
    {
        if (m_Entity.Valid())
        {
            return m_Entity.Get<TagComponent>().tag;
        }
        return m_Name;
    }
    void  SceneHierarchyNode::SetName(const std::string& name) { m_Name = name; }
    void  SceneHierarchyNode::SetEntity(const Entity& entity) { m_Entity = entity; }
    bool  SceneHierarchyNode::IsEntity() const { return m_Entity.Valid(); }
    bool& SceneHierarchyNode::Selected() { return m_Selected; }
    bool  SceneHierarchyNode::IsAnsestor(SceneHierarchyNode* node) const
    {
        const SceneHierarchyNode* ptr = this;
        while (ptr->m_Parent)
        {
            if (ptr->m_Parent == node)
            {
                return true;
            }
            ptr = ptr->m_Parent;
        }
        return false;
    }

    std::vector<Ref<SceneHierarchyNode>>::iterator SceneHierarchyNode::begin() { return m_Childs.begin(); }
    std::vector<Ref<SceneHierarchyNode>>::iterator SceneHierarchyNode::end() { return m_Childs.end(); }

}  // namespace DE