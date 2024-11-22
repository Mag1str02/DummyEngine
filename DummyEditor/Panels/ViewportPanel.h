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
        U32  GetWidth() const { return viewport_size_.x; }
        U32  GetHeight() const { return viewport_size_.y; }
        void UseDepthAttachment(bool f) { use_depth_attachment_ = f; }

    private:
        bool                 use_depth_attachment_ = false;
        ImVec2               viewport_size_;
        WeakRef<FrameBuffer> frame_buffer_;
    };

}  // namespace DummyEngine