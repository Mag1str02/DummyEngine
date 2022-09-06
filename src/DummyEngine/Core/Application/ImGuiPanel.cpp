#include <ImGui/imgui.h>

#include "Core/Application/ImGuiPanel.h"

namespace DE
{
    void ImGuiPanel::OnImGuiRender()
    {
        if (*m_Enabled)
        {
            PushStyle();
            ImGui::Begin(m_Name.c_str(), m_Enabled);
            View();
            ImGui::End();
            PopStyle();
        }
    }

}  // namespace DE