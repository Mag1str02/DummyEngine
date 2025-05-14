#pragma once

#include <vector>
#include "DummyEngine/Core/ECS/Entity.h"
#include "DummyEngine/Core/UI/UIWidgetComponent.h"

namespace DummyEngine {

class UIWidgetHierarchyComponent {
public:
    UIWidgetHierarchyComponent() = default;
    ~UIWidgetHierarchyComponent() = default;

    // Основные методы управления иерархией
    bool SetParent(Entity parent);
    bool AddChild(Entity child);
    bool RemoveChild(Entity child);
    bool IsChildOf(Entity parent) const;
    bool IsParentOf(Entity child) const;
    bool HasChild(Entity child) const;
    bool IsRoot() const { return !m_parent.Valid(); }

    // Геттеры
    Entity GetParent() const { return m_parent; }
    const std::vector<Entity>& GetChildren() const { return m_children; }
    size_t GetChildCount() const { return m_children.size(); }
    Entity GetChild(size_t index) const { return index < m_children.size() ? m_children[index] : Entity(); }

    // Валидация иерархии
    bool ValidateHierarchy() const;
    bool IsValidParent(Entity parent) const;
    bool IsValidChild(Entity child) const;

    // Обновление трансформации
    void UpdateTransform();

private:
    Entity m_parent;
    std::vector<Entity> m_children;
};

} // namespace DummyEngine 