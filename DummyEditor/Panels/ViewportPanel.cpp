#include "DummyEditor/Panels/ViewportPanel.h"

namespace DE {
    void ViewportPanel::OnImGui() {
        DE_PROFILE_SCOPE("ViewportPanel OnImGui");
        if (m_Controller) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            if (ImGui::Begin(ICON_MD_TV "  Viewport")) {
                m_ViewportSize = ImGui::GetContentRegionAvail();
                auto buffer    = m_FrameBuffer.lock();
                if (buffer) {
                    ImGui::Image(reinterpret_cast<void*>(buffer->GetColorAttachment(0)->RendererId()), m_ViewportSize, {0, 1}, {1, 0});
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
    void ViewportPanel::SetFrameBuffer(Ref<FrameBuffer> buffer) {
        m_FrameBuffer = buffer;
    }
}  // namespace DE