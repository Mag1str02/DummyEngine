#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

#include "DummyEngine/Utils/Types/Types.h"

#include <imgui.h>

namespace DummyEngine {

    class FrameBuffer;

    class ViewportPanel : public ImGuiPanel {
    public:
        ViewportPanel() : ImGuiPanel("Viewport") {}
        explicit ViewportPanel(const std::string& panel_name) : ImGuiPanel(panel_name) {}
        virtual void OnImGui() override;

        void ToolPanel();
        void SetFrameBuffer(Ref<FrameBuffer> buffer);
        U32  GetWidth() const { return m_ViewportSize.x; }
        U32  GetHeight() const { return m_ViewportSize.y; }
        void UseDepthAttachment(bool f) { m_UseDepthAttachment = f; }

    private:
        bool                 m_UseDepthAttachment = false;
        ImVec2               m_ViewportSize;
        WeakRef<FrameBuffer> m_FrameBuffer;
    };

}  // namespace DummyEngine