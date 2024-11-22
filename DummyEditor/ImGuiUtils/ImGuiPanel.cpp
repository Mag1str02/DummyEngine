#include "ImGuiPanel.h"

namespace DummyEngine {

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

}  // namespace DummyEngine