#pragma once

#include "UIWidgetComponent.h"
#include <string>
#include <functional>

namespace DummyEngine {

class UICheckbox : public UIWidgetComponent {
public:
    UICheckbox() = default;
    ~UICheckbox() = default;

    void SetCheckedTexture(const std::string& texturePath) { m_checkedTexture = texturePath; }
    void SetUncheckedTexture(const std::string& texturePath) { m_uncheckedTexture = texturePath; }
    void SetText(const std::string& text) { m_text = text; }
    void SetChecked(bool checked) { m_isChecked = checked; }
    void SetOnStateChangedCallback(std::function<void(bool)> callback) { m_onStateChangedCallback = callback; }

    const std::string& GetCheckedTexture() const { return m_checkedTexture; }
    const std::string& GetUncheckedTexture() const { return m_uncheckedTexture; }
    const std::string& GetText() const { return m_text; }
    bool IsChecked() const { return m_isChecked; }

    void OnMouseClick();

    UIWidgetType GetType() const override { return UIWidgetType::Checkbox; }

private:
    std::string m_checkedTexture;
    std::string m_uncheckedTexture;
    std::string m_text;
    bool m_isChecked = false;
    std::function<void(bool)> m_onStateChangedCallback;
};

} // namespace DummyEngine 