#include "UITextField.h"
#include "UIEventSystem.h"

namespace DummyEngine {

void UITextField::OnMouseClick() {
    m_isFocused = true;
}

void UITextField::OnTextInput(const std::string& text) {
    if (!m_isFocused) return;

    m_text += text;
    if (m_onTextChangedCallback) {
        m_onTextChangedCallback(m_text);
    }
}

void UITextField::OnKeyPress(int key) {
    if (!m_isFocused) return;

    // Обработка специальных клавиш (Backspace, Delete и т.д.)
    if (key == 8) { // Backspace
        if (!m_text.empty()) {
            m_text.pop_back();
            if (m_onTextChangedCallback) {
                m_onTextChangedCallback(m_text);
            }
        }
    }
}

} // namespace DummyEngine 