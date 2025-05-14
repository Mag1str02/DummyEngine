#include "UICheckbox.h"
#include "UIEventSystem.h"

namespace DummyEngine {

void UICheckbox::OnMouseClick() {
    m_isChecked = !m_isChecked;
    if (m_onStateChangedCallback) {
        m_onStateChangedCallback(m_isChecked);
    }
}

} // namespace DummyEngine 