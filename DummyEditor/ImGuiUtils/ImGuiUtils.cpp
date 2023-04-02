#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    void ImGuiItem::Enable() {
        m_Controller = true;
    }
    void ImGuiItem::Disable() {
        m_Controller = false;
    }
    bool& ImGuiItem::GetController() {
        return m_Controller;
    }
    const std::string& ImGuiItem::GetName() const {
        return m_Name;
    }
}  // namespace DE