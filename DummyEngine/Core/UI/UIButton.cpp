#include "UIButton.h"
#include "UIEventSystem.h"

namespace DummyEngine {

void UIButton::OnMouseEnter() {
    m_state = ButtonState::Hovered;
}

void UIButton::OnMouseLeave() {
    m_state = ButtonState::Normal;
}

void UIButton::OnMousePress() {
    m_state = ButtonState::Pressed;
}

void UIButton::OnMouseRelease() {
    if (m_state == ButtonState::Pressed) {
        m_state = ButtonState::Hovered;
        if (m_onClickCallback) {
            m_onClickCallback();
        }
    }
}

} // namespace DummyEngine 