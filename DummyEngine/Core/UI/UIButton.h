#pragma once

#include "UIWidgetComponent.h"
#include <string>
#include <functional>

namespace DummyEngine {

enum class ButtonState {
    Normal,
    Hovered,
    Pressed
};

class UIButton : public UIWidgetComponent {
public:
    UIButton() = default;
    ~UIButton() = default;

    void SetNormalTexture(const std::string& texturePath) { m_normalTexture = texturePath; }
    void SetHoveredTexture(const std::string& texturePath) { m_hoveredTexture = texturePath; }
    void SetPressedTexture(const std::string& texturePath) { m_pressedTexture = texturePath; }
    void SetText(const std::string& text) { m_text = text; }
    void SetOnClickCallback(std::function<void()> callback) { m_onClickCallback = callback; }

    const std::string& GetNormalTexture() const { return m_normalTexture; }
    const std::string& GetHoveredTexture() const { return m_hoveredTexture; }
    const std::string& GetPressedTexture() const { return m_pressedTexture; }
    const std::string& GetText() const { return m_text; }
    ButtonState GetState() const { return m_state; }

    void OnMouseEnter();
    void OnMouseLeave();
    void OnMousePress();
    void OnMouseRelease();

    UIWidgetType GetType() const override { return UIWidgetType::Button; }

private:
    std::string m_normalTexture;
    std::string m_hoveredTexture;
    std::string m_pressedTexture;
    std::string m_text;
    ButtonState m_state = ButtonState::Normal;
    std::function<void()> m_onClickCallback;
};

} // namespace DummyEngine 