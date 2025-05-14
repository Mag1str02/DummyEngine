#pragma once

#include "UIWidgetComponent.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include <string>

namespace DummyEngine {

class UIRenderer;

class UIText : public UIWidgetComponent {
public:
    UIText() = default;
    ~UIText() = default;

    // Установка текста
    void SetText(const std::string& text) { m_text = text; }
    const std::string& GetText() const { return m_text; }

    // Установка шрифта
    void SetFont(const std::string& fontName) { m_fontName = fontName; }
    void SetFontName(const std::string& fontName);
    const std::string& GetFont() const { return m_fontName; }

    // Установка цвета текста
    void SetTextColor(const glm::vec4& color) { m_textColor = color; }
    const glm::vec4& GetTextColor() const { return m_textColor; }

    // Получение размеров текста
    glm::vec2 GetTextSize() const;

    UIWidgetType GetType() const override { return UIWidgetType::Text; }

    void Render(UIRenderer& renderer);

private:
    std::string m_text;
    std::string m_fontName = "Default";
    glm::vec4 m_textColor = glm::vec4(1.0f);
};

} // namespace DummyEngine 