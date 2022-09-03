#include "Panels/ViewportPanel.h"

namespace DE
{
    ViewportPanel::ViewportPanel() : m_ViewportSize(ImVec2(1, 1)) {}

    void ViewportPanel::OnImGuiRender(const Ref<FrameBuffer> buffer)
    {
        ImGui::Begin("Viewport");
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        // ImGui::PushStyleColor(ImGuiColor.Border, ImVec3(1.0f, 1.0f, 1.0f));
        m_ViewportSize = ImGui::GetContentRegionAvail();
        // std::cout << "Vieport size: " << m_ViewportSize.x << " x " << m_ViewportSize.y << std::endl;

        ImGui::Image(reinterpret_cast<void*>(buffer->GetColorAttachment(0)->RendererId()), m_ViewportSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::PopStyleVar();

        ImGui::End();
    }
}  // namespace DE