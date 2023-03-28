#include "DummyEditor/Panels/ViewportPanel.h"

namespace DE {
    void ViewportPanel::View() {
        DE_PROFILE_SCOPE("ViewportPanel View");

        m_ViewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image(reinterpret_cast<void*>(m_FrameBuffer->GetColorAttachment(0)->RendererId()), m_ViewportSize, {0, 1}, {1, 0});
    }
    void ViewportPanel::SetFrameBuffer(Ref<FrameBuffer> buffer) {
        m_FrameBuffer = buffer;
    }
}  // namespace DE