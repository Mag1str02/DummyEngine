#include "UIText.h"
#include "UIFontManager.h"
#include "UIRenderer.h"

namespace DummyEngine {

void UIText::SetFontName(const std::string& fontName) {
    m_fontName = fontName;
    // Обновляем размер виджета при смене шрифта
    auto textSize = GetTextSize();
    SetSize(textSize);
}

glm::vec2 UIText::GetTextSize() const {
    return UIFontManager::GetInstance().CalculateTextSize(m_text, m_fontName);
}

void UIText::Render(UIRenderer& renderer) {
    // Получаем позицию виджета
    auto position = GetPosition();
    
    // Рендерим текст
    renderer.RenderText(m_text, m_fontName, m_textColor, position);
}

} // namespace DummyEngine 