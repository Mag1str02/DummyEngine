#include "UIWidgetEditor.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/Components.h"
#include <imgui.h>

namespace DummyEngine {

void UIWidgetEditor::DrawWidgetHierarchy(Entity widget) {
    if (!widget.Valid()) return;

    ImGui::Begin("UI Widget Hierarchy");
    
    // Рисуем корневой виджет
    if (UIWidgetSystem::GetInstance().IsRootWidget(widget)) {
        DrawWidgetNode(widget);
    }

    ImGui::End();
}

void UIWidgetEditor::DrawWidgetNode(Entity widget) {
    if (!widget.Valid()) return;

    auto& widgetComponent = widget.Get<UIWidgetComponent>();
    auto& hierarchy = widget.Get<UIWidgetHierarchyComponent>();

    // Создаем уникальный ID для узла
    ImGui::PushID(widget.Get<IDComponent>().Hex().c_str());

    // Определяем иконку в зависимости от типа виджета
    const char* icon = "□"; // По умолчанию
    switch (widgetComponent.GetType()) {
        case UIWidgetType::Container: icon = "⊞"; break;
        case UIWidgetType::Text: icon = "T"; break;
        case UIWidgetType::Button: icon = "⬡"; break;
        case UIWidgetType::Checkbox: icon = "☐"; break;
        case UIWidgetType::TextField: icon = "⌨"; break;
    }

    // Создаем флаг для узла
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (hierarchy.GetChildCount() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (widget == UIWidgetSystem::GetInstance().GetRootWidget()) {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }

    // Рисуем узел
    bool isOpen = ImGui::TreeNodeEx((icon + std::string(" ") + widgetComponent.GetName()).c_str(), flags);
    
    // Обработка drag & drop
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("UI_WIDGET", &widget, sizeof(Entity));
        ImGui::Text("Move %s", widgetComponent.GetName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UI_WIDGET")) {
            Entity sourceWidget = *(Entity*)payload->Data;
            if (sourceWidget.Valid() && sourceWidget != widget) {
                UIWidgetSystem::GetInstance().SetWidgetParent(sourceWidget, widget);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Рисуем дочерние элементы
    if (isOpen) {
        for (const auto& child : hierarchy.GetChildren()) {
            DrawWidgetNode(child);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void UIWidgetEditor::DrawWidgetProperties(Entity widget) {
    if (!widget.Valid()) return;

    ImGui::Begin("Widget Properties");

    auto& widgetComponent = widget.Get<UIWidgetComponent>();

    // Общие свойства
    ImGui::Text("Widget Type: %s", [&]() {
        switch (widgetComponent.GetType()) {
            case UIWidgetType::Container: return "Container";
            case UIWidgetType::Text: return "Text";
            case UIWidgetType::Button: return "Button";
            case UIWidgetType::Checkbox: return "Checkbox";
            case UIWidgetType::TextField: return "TextField";
            default: return "Unknown";
        }
    }());

    // Имя виджета
    char name[256];
    strcpy(name, widgetComponent.GetName().c_str());
    if (ImGui::InputText("Name", name, sizeof(name))) {
        widgetComponent.SetName(name);
    }

    // Видимость
    bool visible = widgetComponent.IsVisible();
    if (ImGui::Checkbox("Visible", &visible)) {
        widgetComponent.SetVisible(visible);
    }

    // Позиция и размер
    glm::vec2 position = widgetComponent.GetPosition();
    if (ImGui::DragFloat2("Position", &position.x, 0.01f)) {
        widgetComponent.SetPosition(position);
    }

    glm::vec2 size = widgetComponent.GetSize();
    if (ImGui::DragFloat2("Size", &size.x, 0.01f)) {
        widgetComponent.SetSize(size);
    }

    // Цвет
    glm::vec4 color = widgetComponent.GetColor();
    if (ImGui::ColorEdit4("Color", &color.x)) {
        widgetComponent.SetColor(color);
    }

    // Специфичные свойства для каждого типа виджета
    DrawWidgetTypeProperties(widget);

    ImGui::End();
}

void UIWidgetEditor::DrawWidgetTypeProperties(Entity widget) {
    if (!widget.Valid()) return;

    auto& widgetComponent = widget.Get<UIWidgetComponent>();
    switch (widgetComponent.GetType()) {
        case UIWidgetType::Container:
            DrawContainerProperties(widget);
            break;
        case UIWidgetType::Text:
            DrawTextProperties(widget);
            break;
        case UIWidgetType::Button:
            DrawButtonProperties(widget);
            break;
        case UIWidgetType::Checkbox:
            DrawCheckboxProperties(widget);
            break;
        case UIWidgetType::TextField:
            DrawTextFieldProperties(widget);
            break;
    }
}

void UIWidgetEditor::DrawContainerProperties(Entity widget) {
    if (!widget.Has<UIContainer>()) return;
    auto& container = widget.Get<UIContainer>();

    // Фоновая текстура
    static char texturePath[256] = "";
    if (ImGui::InputText("Background Texture", texturePath, sizeof(texturePath))) {
        container.SetBackgroundTexture(texturePath);
    }
}

void UIWidgetEditor::DrawTextProperties(Entity widget) {
    if (!widget.Has<UIText>()) return;
    auto& text = widget.Get<UIText>();

    // Текст
    static char textContent[1024] = "";
    if (ImGui::InputTextMultiline("Text", textContent, sizeof(textContent))) {
        text.SetText(textContent);
    }

    // Шрифт
    static char fontPath[256] = "";
    if (ImGui::InputText("Font", fontPath, sizeof(fontPath))) {
        text.SetFont(fontPath);
    }

    // Цвет текста
    glm::vec4 textColor = text.GetTextColor();
    if (ImGui::ColorEdit4("Text Color", &textColor.x)) {
        text.SetTextColor(textColor);
    }
}

void UIWidgetEditor::DrawButtonProperties(Entity widget) {
    if (!widget.Has<UIButton>()) return;
    auto& button = widget.Get<UIButton>();

    // Текст кнопки
    static char buttonText[256] = "";
    if (ImGui::InputText("Button Text", buttonText, sizeof(buttonText))) {
        button.SetText(buttonText);
    }

    // Текстуры состояний
    static char normalTexture[256] = "";
    if (ImGui::InputText("Normal Texture", normalTexture, sizeof(normalTexture))) {
        button.SetNormalTexture(normalTexture);
    }

    static char hoveredTexture[256] = "";
    if (ImGui::InputText("Hovered Texture", hoveredTexture, sizeof(hoveredTexture))) {
        button.SetHoveredTexture(hoveredTexture);
    }

    static char pressedTexture[256] = "";
    if (ImGui::InputText("Pressed Texture", pressedTexture, sizeof(pressedTexture))) {
        button.SetPressedTexture(pressedTexture);
    }
}

void UIWidgetEditor::DrawCheckboxProperties(Entity widget) {
    if (!widget.Has<UICheckbox>()) return;
    auto& checkbox = widget.Get<UICheckbox>();

    // Состояние чекбокса
    bool checked = checkbox.IsChecked();
    if (ImGui::Checkbox("Checked", &checked)) {
        checkbox.SetChecked(checked);
    }

    // Текст чекбокса
    static char checkboxText[256] = "";
    if (ImGui::InputText("Checkbox Text", checkboxText, sizeof(checkboxText))) {
        checkbox.SetText(checkboxText);
    }
}

void UIWidgetEditor::DrawTextFieldProperties(Entity widget) {
    if (!widget.Has<UITextField>()) return;
    auto& textField = widget.Get<UITextField>();

    // Текст поля
    static char fieldText[1024] = "";
    if (ImGui::InputTextMultiline("Field Text", fieldText, sizeof(fieldText))) {
        textField.SetText(fieldText);
    }

    // Плейсхолдер
    static char placeholder[256] = "";
    if (ImGui::InputText("Placeholder", placeholder, sizeof(placeholder))) {
        textField.SetPlaceholder(placeholder);
    }
}

void UIWidgetEditor::DrawWidgetCreationMenu(Entity parent) {
    if (ImGui::BeginMenu("Create Widget")) {
        if (ImGui::MenuItem("Container")) {
            Entity widget = UIWidgetSystem::GetInstance().CreateWidget(UIWidgetType::Container);
            if (parent.Valid()) {
                UIWidgetSystem::GetInstance().SetWidgetParent(widget, parent);
            }
        }
        if (ImGui::MenuItem("Text")) {
            Entity widget = UIWidgetSystem::GetInstance().CreateWidget(UIWidgetType::Text);
            if (parent.Valid()) {
                UIWidgetSystem::GetInstance().SetWidgetParent(widget, parent);
            }
        }
        if (ImGui::MenuItem("Button")) {
            Entity widget = UIWidgetSystem::GetInstance().CreateWidget(UIWidgetType::Button);
            if (parent.Valid()) {
                UIWidgetSystem::GetInstance().SetWidgetParent(widget, parent);
            }
        }
        if (ImGui::MenuItem("Checkbox")) {
            Entity widget = UIWidgetSystem::GetInstance().CreateWidget(UIWidgetType::Checkbox);
            if (parent.Valid()) {
                UIWidgetSystem::GetInstance().SetWidgetParent(widget, parent);
            }
        }
        if (ImGui::MenuItem("TextField")) {
            Entity widget = UIWidgetSystem::GetInstance().CreateWidget(UIWidgetType::TextField);
            if (parent.Valid()) {
                UIWidgetSystem::GetInstance().SetWidgetParent(widget, parent);
            }
        }
        ImGui::EndMenu();
    }
}

void UIWidgetEditor::DrawWidgetDragDrop(Entity source, Entity target) {
    if (!source.Valid() || !target.Valid()) return;

    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("UI_WIDGET", &source, sizeof(Entity));
        ImGui::Text("Move %s", source.Get<UIWidgetComponent>().GetName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UI_WIDGET")) {
            Entity draggedWidget = *(Entity*)payload->Data;
            if (draggedWidget.Valid() && draggedWidget != target) {
                UIWidgetSystem::GetInstance().SetWidgetParent(draggedWidget, target);
            }
        }
        ImGui::EndDragDropTarget();
    }
}

} // namespace DummyEngine 