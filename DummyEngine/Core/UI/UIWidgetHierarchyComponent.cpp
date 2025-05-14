#include "DummyEngine/Core/UI/UIWidgetHierarchyComponent.h"
#include <algorithm>

namespace DummyEngine {

bool UIWidgetHierarchyComponent::SetParent(Entity parent) {
    if (!IsValidParent(parent)) {
        return false;
    }

    // Удаляем из старого родителя
    if (m_parent.Valid()) {
        auto& oldParentHierarchy = m_parent.Get<UIWidgetHierarchyComponent>();
        oldParentHierarchy.RemoveChild(m_parent);
    }

    // Устанавливаем нового родителя
    m_parent = parent;
    if (parent.Valid()) {
        auto& newParentHierarchy = parent.Get<UIWidgetHierarchyComponent>();
        newParentHierarchy.AddChild(m_parent);
    }

    return true;
}

bool UIWidgetHierarchyComponent::AddChild(Entity child) {
    if (!IsValidChild(child)) {
        return false;
    }

    // Проверяем, не является ли child уже дочерним элементом
    if (HasChild(child)) {
        return false;
    }

    // Устанавливаем родителя для child
    auto& childHierarchy = child.Get<UIWidgetHierarchyComponent>();
    if (!childHierarchy.SetParent(m_parent)) {
        return false;
    }

    m_children.push_back(child);
    return true;
}

bool UIWidgetHierarchyComponent::RemoveChild(Entity child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it == m_children.end()) {
        return false;
    }

    // Удаляем родителя у child
    if (child.Valid()) {
        auto& childHierarchy = child.Get<UIWidgetHierarchyComponent>();
        childHierarchy.SetParent(Entity());
    }

    m_children.erase(it);
    return true;
}

bool UIWidgetHierarchyComponent::IsChildOf(Entity parent) const {
    return m_parent == parent;
}

bool UIWidgetHierarchyComponent::IsParentOf(Entity child) const {
    return HasChild(child);
}

bool UIWidgetHierarchyComponent::HasChild(Entity child) const {
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

bool UIWidgetHierarchyComponent::ValidateHierarchy() const {

    return true;
}

bool UIWidgetHierarchyComponent::IsValidParent(Entity parent) const {
    if (!parent.Valid()) {
        return true; // Корневой виджет может не иметь родителя
    }

    // Проверяем, что parent имеет необходимые компоненты
    if (!parent.Has<UIWidgetHierarchyComponent>() || !parent.Has<UIWidgetComponent>()) {
        return false;
    }

    // Проверяем, что parent не является дочерним элементом текущего виджета
    auto& parentHierarchy = parent.Get<UIWidgetHierarchyComponent>();
    if (parentHierarchy.IsChildOf(m_parent)) {
        return false;
    }

    return true;
}

bool UIWidgetHierarchyComponent::IsValidChild(Entity child) const {
    if (!child.Valid()) {
        return false;
    }

    // Проверяем, что child имеет необходимые компоненты
    if (!child.Has<UIWidgetHierarchyComponent>() || !child.Has<UIWidgetComponent>()) {
        return false;
    }

    // Проверяем, что child не является родителем текущего виджета
    if (child == m_parent) {
        return false;
    }

    return true;
}

void UIWidgetHierarchyComponent::UpdateTransform() {
    if (!m_parent.Valid()) return;

    // Обновляем позицию и размер всех дочерних элементов
    for (auto& child : m_children) {
        if (child.Valid() && child.Has<UIWidgetComponent>()) {
            UIWidgetComponent& childWidget = child.Get<UIWidgetComponent>();
            const UIWidgetComponent& parentWidget = m_parent.Get<UIWidgetComponent>();
            
            // Обновляем позицию и размер относительно родителя
            glm::vec2 newPos = parentWidget.GetPosition() + childWidget.GetPosition() * parentWidget.GetSize();
            glm::vec2 newSize = childWidget.GetSize() * parentWidget.GetSize();
            
            childWidget.SetPosition(newPos);
            childWidget.SetSize(newSize);
        }
    }
}

} // namespace DummyEngine 