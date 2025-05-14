#pragma once

#include <variant>
#include <string>
#include <glm/glm.hpp>
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"

namespace DummyEngine {

enum class UIWidgetType {
    Container,
    Text,
    Button,
    Checkbox,
    TextField
};

struct UIWidgetData {
    glm::vec2 position;  // Позиция относительно родителя (в процентах)
    glm::vec2 size;      // Размер относительно родителя (в процентах)
    bool isVisible;
    std::string name;
    glm::vec4 color = glm::vec4(1.0f); // Цвет виджета
    Ref<Texture> texture; // Текстура виджета
};

class UIWidgetComponent {
public:
    UIWidgetComponent() = default;
    virtual ~UIWidgetComponent() = default;

    void SetPosition(const glm::vec2& pos) { m_data.position = pos; }
    void SetSize(const glm::vec2& size) { m_data.size = size; }
    void SetVisible(bool visible) { m_data.isVisible = visible; }
    void SetName(const std::string& name) { m_data.name = name; }
    void SetColor(const glm::vec4& color) { m_data.color = color; }
    void SetTexture(Ref<Texture> texture) { m_data.texture = texture; }
    void SetRootWidget(Ref<UIWidgetComponent> widget) { m_rootWidget = widget; }

    const glm::vec2& GetPosition() const { return m_data.position; }
    const glm::vec2& GetSize() const { return m_data.size; }
    bool IsVisible() const { return m_data.isVisible; }
    const std::string& GetName() const { return m_data.name; }
    const glm::vec4& GetColor() const { return m_data.color; }
    Ref<Texture> GetTexture() const { return m_data.texture; }
    Ref<UIWidgetComponent> GetRootWidget() const { return m_rootWidget; }

    virtual UIWidgetType GetType() const = 0;

protected:
    UIWidgetData m_data;
    Ref<UIWidgetComponent> m_rootWidget;
};

} // namespace DummyEngine 