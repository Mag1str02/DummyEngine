#pragma once

#include <memory>
#include <vector>
#include "UIWidgetComponent.h"
#include "UIWidgetHierarchyComponent.h"
#include "DummyEngine/Core/ECS/ECS.h"

namespace DummyEngine {

class UIWidgetSystem {
public:
    static UIWidgetSystem& GetInstance();

    void Initialize();
    void Shutdown();

    void Update(float deltaTime);
    void Render();

    Entity CreateWidget(UIWidgetType type);
    void DestroyWidget(Entity widget);

    bool SetRootWidget(Entity widget);
    Entity GetRootWidget() const { return m_rootWidget; }
    bool IsRootWidget(Entity widget) const { return widget == m_rootWidget; }

    bool SetWidgetParent(Entity widget, Entity parent);
    bool AddWidgetChild(Entity parent, Entity child);
    bool RemoveWidgetChild(Entity parent, Entity child);
    bool ValidateWidgetHierarchy(Entity widget) const;
    std::vector<Entity> GetWidgetChildren(Entity widget) const;
    Entity GetWidgetParent(Entity widget) const;

    bool IsValidWidget(Entity widget) const;
    bool IsWidgetVisible(Entity widget) const;
    void SetWidgetVisible(Entity widget, bool visible);

private:
    UIWidgetSystem() = default;
    ~UIWidgetSystem() = default;

    void UpdateWidgetTransform(Entity widget);
    void UpdateWidgetState(Entity widget);
    void RenderWidget(Entity widget);

    Entity m_rootWidget;
    std::vector<Entity> m_widgets;
};

} // namespace DummyEngine 