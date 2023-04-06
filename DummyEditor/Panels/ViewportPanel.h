#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class ViewportPanel : public ImGuiPanel {
    public:
        ViewportPanel() : ImGuiPanel("Viewport") {}
        virtual void OnImGui() override;

        void SetFrameBuffer(Ref<FrameBuffer> buffer);
        U32  GetWidth() const { return m_ViewportSize.x; }
        U32  GetHeight() const { return m_ViewportSize.y; }

    private:
        ImVec2           m_ViewportSize;
        Ref<FrameBuffer> m_FrameBuffer;
    };
}  // namespace DE