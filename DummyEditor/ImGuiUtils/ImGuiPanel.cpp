#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    void ImGuiPanel::OnImGui() {
        if (m_Controller) {
            PushStyle();
            ImGui::Begin(m_Name.c_str(), &m_Controller);
            View();
            ImGui::End();
            PopStyle();
        }
    }
}  // namespace DE