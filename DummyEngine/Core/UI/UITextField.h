#pragma once

#include "UIWidgetComponent.h"
#include <string>
#include <functional>

namespace DummyEngine {

class UITextField : public UIWidgetComponent {
public:
    UITextField() = default;
    ~UITextField() = default;

    void SetBackgroundTexture(const std::string& texturePath) { m_backgroundTexture = texturePath; }
    void SetText(const std::string& text) { m_text = text; }
    void SetFont(const std::string& fontPath) { m_fontPath = fontPath; }
    void SetFontSize(float size) { m_fontSize = size; }
    void SetTextColor(const glm::vec4& color) { m_textColor = color; }
    void SetPlaceholder(const std::string& placeholder) { m_placeholder = placeholder; }
    void SetOnTextChangedCallback(std::function<void(const std::string&)> callback) { m_onTextChangedCallback = callback; }

    const std::string& GetBackgroundTexture() const { return m_backgroundTexture; }
    const std::string& GetText() const { return m_text; }
    const std::string& GetFont() const { return m_fontPath; }
    float GetFontSize() const { return m_fontSize; }
    const glm::vec4& GetTextColor() const { return m_textColor; }
    const std::string& GetPlaceholder() const { return m_placeholder; }
    bool IsFocused() const { return m_isFocused; }

    void OnMouseClick();
    void OnTextInput(const std::string& text);
    void OnKeyPress(int key);

    UIWidgetType GetType() const override { return UIWidgetType::TextField; }

private:
    std::string m_backgroundTexture;
    std::string m_text;
    std::string m_fontPath;
    float m_fontSize = 16.0f;
    glm::vec4 m_textColor = glm::vec4(1.0f);
    std::string m_placeholder;
    bool m_isFocused = false;
    std::function<void(const std::string&)> m_onTextChangedCallback;
};

} // namespace DummyEngine 