#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    void ImGuiPanel::Enable() {
        m_Controller = true;
    }
    void ImGuiPanel::Disable() {
        m_Controller = false;
    }
    bool& ImGuiPanel::GetController() {
        return m_Controller;
    }
    const std::string& ImGuiPanel::GetName() const {
        return m_Name;
    }
}  // namespace DE