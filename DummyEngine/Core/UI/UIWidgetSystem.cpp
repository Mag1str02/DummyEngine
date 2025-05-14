#include "UIWidgetSystem.h"
#include "UIRenderer.h"
#include "UIEventSystem.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/ECS/ECS.h"
#include "UIContainer.h"
#include "UIText.h"
#include "UIButton.h"
#include "UICheckbox.h"
#include "UITextField.h"

namespace DummyEngine {

UIWidgetSystem& UIWidgetSystem::GetInstance() {
    static UIWidgetSystem instance;
    return instance;
}

void UIWidgetSystem::Initialize() {
    LOG_INFO("UIWidgetSystem: Initializing widget system");
    UIRenderer::GetInstance().Initialize();
    LOG_INFO("UIWidgetSystem: Widget system initialized successfully");
}

void UIWidgetSystem::Shutdown() {
    LOG_INFO("UIWidgetSystem: Shutting down widget system");
    UIRenderer::GetInstance().Shutdown();
    m_widgets.clear();
    LOG_INFO("UIWidgetSystem: Widget system shut down successfully");
}

void UIWidgetSystem::Update(float /*deltaTime*/) {
    if (!m_rootWidget.Valid()) {
        LOG_WARNING("UIWidgetSystem: No valid root widget found, skipping update");
        return;
    }

    LOG_INFO("UIWidgetSystem: Starting widget update");
    // Обновляем трансформации и состояния всех виджетов
    for (auto& widget : m_widgets) {
        if (widget.Valid()) {
            LOG_INFO("UIWidgetSystem: Updating widget (Entity ID: {})", widget.Get<IDComponent>().Hex());
            UpdateWidgetTransform(widget);
            UpdateWidgetState(widget);
        } else {
            LOG_WARNING("UIWidgetSystem: Found invalid widget in update loop");
        }
    }
    LOG_INFO("UIWidgetSystem: Widget update completed");
}

void UIWidgetSystem::Render() {
    if (!m_rootWidget.Valid()) {
        LOG_WARNING("UIWidgetSystem: No valid root widget found, skipping render");
        return;
    }

    LOG_INFO("UIWidgetSystem: Starting widget render");
    UIRenderer::GetInstance().BeginFrame();

    // Рендерим все виджеты
    for (auto& widget : m_widgets) {
        if (widget.Valid()) {
            LOG_INFO("UIWidgetSystem: Rendering widget (Entity ID: {})", widget.Get<IDComponent>().Hex());
            RenderWidget(widget);
        } else {
            LOG_WARNING("UIWidgetSystem: Found invalid widget in render loop");
        }
    }

    UIRenderer::GetInstance().EndFrame();
    LOG_INFO("UIWidgetSystem: Widget render completed");
}

Entity UIWidgetSystem::CreateWidget(UIWidgetType type) {
    LOG_INFO("Creating widget of type {}", static_cast<int>(type));
    auto scene = Scene::GetActiveScene();
    if (!scene) {
        LOG_ERROR("No active scene found");
        return Entity();
    }

    Entity widget = scene->CreateEntity();
    LOG_INFO("Created entity for widget");
    
    // Добавляем необходимые компоненты
    widget.Add<UIWidgetHierarchyComponent>();
    LOG_INFO("Added UIWidgetHierarchyComponent");

    // Создаем конкретный виджет в зависимости от типа
    switch (type) {
        case UIWidgetType::Container: {
            LOG_INFO("Creating UIContainer");
            auto& container = widget.Add<UIContainer>();
            container.SetPosition(glm::vec2(0.0f));
            container.SetSize(glm::vec2(1.0f));
            container.SetVisible(true);
            container.SetName("Container");
            container.SetColor(glm::vec4(1.0f));
            LOG_INFO("UIContainer created and initialized");
            break;
        }
        case UIWidgetType::Text: {
            LOG_INFO("Creating UIText");
            auto& text = widget.Add<UIText>();
            text.SetPosition(glm::vec2(0.0f));
            text.SetSize(glm::vec2(1.0f));
            text.SetVisible(true);
            text.SetName("Text");
            text.SetColor(glm::vec4(1.0f));
            LOG_INFO("UIText created and initialized");
            break;
        }
        case UIWidgetType::Button: {
            LOG_INFO("Creating UIButton");
            auto& button = widget.Add<UIButton>();
            button.SetPosition(glm::vec2(0.0f));
            button.SetSize(glm::vec2(1.0f));
            button.SetVisible(true);
            button.SetName("Button");
            button.SetColor(glm::vec4(1.0f));
            LOG_INFO("UIButton created and initialized");
            break;
        }
        case UIWidgetType::Checkbox: {
            LOG_INFO("Creating UICheckbox");
            auto& checkbox = widget.Add<UICheckbox>();
            checkbox.SetPosition(glm::vec2(0.0f));
            checkbox.SetSize(glm::vec2(1.0f));
            checkbox.SetVisible(true);
            checkbox.SetName("Checkbox");
            checkbox.SetColor(glm::vec4(1.0f));
            LOG_INFO("UICheckbox created and initialized");
            break;
        }
        case UIWidgetType::TextField: {
            LOG_INFO("Creating UITextField");
            auto& textField = widget.Add<UITextField>();
            textField.SetPosition(glm::vec2(0.0f));
            textField.SetSize(glm::vec2(1.0f));
            textField.SetVisible(true);
            textField.SetName("TextField");
            textField.SetColor(glm::vec4(1.0f));
            LOG_INFO("UITextField created and initialized");
            break;
        }
    }

    m_widgets.push_back(widget);
    LOG_INFO("Widget added to system");
    return widget;
}

void UIWidgetSystem::DestroyWidget(Entity widget) {
    if (!widget.Valid()) return;

    auto it = std::find(m_widgets.begin(), m_widgets.end(), widget);
    if (it != m_widgets.end()) {
        m_widgets.erase(it);
    }

    // Удаляем виджет из сцены
    // auto scene = Scene::GetActiveScene();
    // if (scene) {
    //     scene->DestroyEntity(widget);
    // }
    widget.Destroy();
}

bool UIWidgetSystem::SetRootWidget(Entity widget) {
    if (!IsValidWidget(widget)) {
        LOG_WARNING("UIWidgetSystem: Attempting to set invalid widget as root");
        return false;
    }

    // Проверяем, что виджет не является дочерним элементом другого виджета
    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();
    if (hierarchy.GetParent().Valid()) {
        LOG_WARNING("UIWidgetSystem: Cannot set widget with parent as root");
        return false;
    }

    m_rootWidget = widget;
    LOG_INFO("UIWidgetSystem: Root widget set successfully (Entity ID: {})", widget.Get<IDComponent>().Hex());
    return true;
}

bool UIWidgetSystem::SetWidgetParent(Entity widget, Entity parent) {
    if (!IsValidWidget(widget)) {
        LOG_WARNING("UIWidgetSystem: Invalid widget in SetWidgetParent");
        return false;
    }

    if (!IsValidWidget(parent)) {
        LOG_WARNING("UIWidgetSystem: Invalid parent in SetWidgetParent");
        return false;
    }

    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();
    if (!hierarchy.SetParent(parent)) {
        LOG_WARNING("UIWidgetSystem: Failed to set parent for widget");
        return false;
    }

    LOG_INFO("UIWidgetSystem: Parent set successfully for widget (Entity ID: {})", widget.Get<IDComponent>().Hex());
    return true;
}

bool UIWidgetSystem::AddWidgetChild(Entity parent, Entity child) {
    if (!IsValidWidget(parent) || !IsValidWidget(child)) {
        LOG_WARNING("UIWidgetSystem: Invalid widget or child in AddWidgetChild");
        return false;
    }

    auto& hierarchy = parent.Get<UIWidgetHierarchyComponent>();
    if (!hierarchy.AddChild(child)) {
        LOG_WARNING("UIWidgetSystem: Failed to add child to widget");
        return false;
    }

    LOG_INFO("UIWidgetSystem: Child added successfully to widget (Entity ID: {})", parent.Get<IDComponent>().Hex());
    return true;
}

bool UIWidgetSystem::RemoveWidgetChild(Entity parent, Entity child) {
    if (!IsValidWidget(parent) || !IsValidWidget(child)) {
        LOG_WARNING("UIWidgetSystem: Invalid widget or child in RemoveWidgetChild");
        return false;
    }

    auto& hierarchy = parent.Get<UIWidgetHierarchyComponent>();
    if (!hierarchy.RemoveChild(child)) {
        LOG_WARNING("UIWidgetSystem: Failed to remove child from widget");
        return false;
    }

    LOG_INFO("UIWidgetSystem: Child removed successfully from widget (Entity ID: {})", parent.Get<IDComponent>().Hex());
    return true;
}

bool UIWidgetSystem::ValidateWidgetHierarchy(Entity widget) const {
    if (!IsValidWidget(widget)) {
        return false;
    }

    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();
    return hierarchy.ValidateHierarchy();
}

std::vector<Entity> UIWidgetSystem::GetWidgetChildren(Entity widget) const {
    if (!IsValidWidget(widget)) {
        return {};
    }

    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();
    return hierarchy.GetChildren();
}

Entity UIWidgetSystem::GetWidgetParent(Entity widget) const {
    if (!IsValidWidget(widget)) {
        return Entity();
    }

    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();
    return hierarchy.GetParent();
}

bool UIWidgetSystem::IsValidWidget(Entity widget) const {
    if (!widget.Valid() || !widget.Has<UIWidgetHierarchyComponent>()) {
        return false;
    }

    // Проверяем наличие конкретного типа виджета
    return widget.Has<UIContainer>() || 
           widget.Has<UIText>() || 
           widget.Has<UIButton>() || 
           widget.Has<UICheckbox>() || 
           widget.Has<UITextField>();
}

bool UIWidgetSystem::IsWidgetVisible(Entity widget) const {
    if (!IsValidWidget(widget)) {
        return false;
    }

    auto& widgetComponent = widget.Get<UIWidgetComponent>();
    return widgetComponent.IsVisible();
}

void UIWidgetSystem::SetWidgetVisible(Entity widget, bool visible) {
    if (!IsValidWidget(widget)) {
        return;
    }

    auto& widgetComponent = widget.Get<UIWidgetComponent>();
    widgetComponent.SetVisible(visible);
}

void UIWidgetSystem::UpdateWidgetTransform(Entity widget) {
    if (!widget.Valid()) return;

    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();
    
    // Получаем компонент виджета в зависимости от его типа
    UIWidgetComponent* widgetComponent = nullptr;
    if (widget.Has<UIContainer>()) {
        widgetComponent = &widget.Get<UIContainer>();
    } else if (widget.Has<UIText>()) {
        widgetComponent = &widget.Get<UIText>();
    } else if (widget.Has<UIButton>()) {
        widgetComponent = &widget.Get<UIButton>();
    } else if (widget.Has<UICheckbox>()) {
        widgetComponent = &widget.Get<UICheckbox>();
    } else if (widget.Has<UITextField>()) {
        widgetComponent = &widget.Get<UITextField>();
    }

    if (!widgetComponent) {
        LOG_ERROR("Widget has no valid component");
        return;
    }

    // Обновляем позицию и размер на основе родительского виджета
    Entity parent = hierarchy.GetParent();
    if (parent.Valid()) {
        UIWidgetComponent* parentComponent = nullptr;
        if (parent.Has<UIContainer>()) {
            parentComponent = &parent.Get<UIContainer>();
        } else if (parent.Has<UIText>()) {
            parentComponent = &parent.Get<UIText>();
        } else if (parent.Has<UIButton>()) {
            parentComponent = &parent.Get<UIButton>();
        } else if (parent.Has<UICheckbox>()) {
            parentComponent = &parent.Get<UICheckbox>();
        } else if (parent.Has<UITextField>()) {
            parentComponent = &parent.Get<UITextField>();
        }

        if (parentComponent) {
            auto parentPos = parentComponent->GetPosition();
            auto parentSize = parentComponent->GetSize();
            widgetComponent->SetPosition(parentPos + widgetComponent->GetPosition() * parentSize);
            widgetComponent->SetSize(widgetComponent->GetSize() * parentSize);
        }
    }

    // Обновляем трансформации дочерних виджетов
    for (const auto& child : hierarchy.GetChildren()) {
        if (child.Valid()) {
            UpdateWidgetTransform(child);
        }
    }
}

void UIWidgetSystem::UpdateWidgetState(Entity widget) {
    if (!widget.Valid()) return;

    // Обновляем состояние виджета в зависимости от его типа
    switch (widget.Get<UIContainer>().GetType()) {
        case UIWidgetType::Button: {
            // auto& button = widget.Get<UIButton>();
            // Обновляем состояние кнопки (hover, pressed и т.д.)
            break;
        }
        case UIWidgetType::Checkbox: {
            // auto& checkbox = widget.Get<UICheckbox>();
            // Обновляем состояние чекбокса
            break;
        }
        case UIWidgetType::TextField: {
            // auto& textField = widget.Get<UITextField>();
            // Обновляем состояние текстового поля
            break;
        }
        default:
            break;
    }
}

void UIWidgetSystem::RenderWidget(Entity widget) {
    if (!widget.Valid()) {
        LOG_WARNING("UIWidgetSystem: Attempting to render invalid widget");
        return;
    }

    // Получаем компонент виджета в зависимости от его типа
    UIWidgetComponent* widgetComponent = nullptr;
    if (widget.Has<UIContainer>()) {
        widgetComponent = &widget.Get<UIContainer>();
    } else if (widget.Has<UIText>()) {
        widgetComponent = &widget.Get<UIText>();
    } else if (widget.Has<UIButton>()) {
        widgetComponent = &widget.Get<UIButton>();
    } else if (widget.Has<UICheckbox>()) {
        widgetComponent = &widget.Get<UICheckbox>();
    } else if (widget.Has<UITextField>()) {
        widgetComponent = &widget.Get<UITextField>();
    }

    if (!widgetComponent) {
        LOG_ERROR("UIWidgetSystem: Widget has no valid component");
        return;
    }

    if (!widgetComponent->IsVisible()) {
        LOG_INFO("UIWidgetSystem: Skipping invisible widget (Entity ID: {})", widget.Get<IDComponent>().Hex());
        return;
    }

    LOG_INFO("UIWidgetSystem: Rendering widget (Entity ID: {}, Type: {})", 
        widget.Get<IDComponent>().Hex(), 
        static_cast<int>(widgetComponent->GetType()));
    
    // Рендерим виджет через UIRenderer
    UIRenderer::GetInstance().RenderWidget(widget);
}

} // namespace DummyEngine 