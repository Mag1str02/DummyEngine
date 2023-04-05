#include "DummyEditor/DummyEngineInclude.h"

namespace DE
{
    class ViewportPanel : public ImGuiPanel
    {
    public:
        ViewportPanel() : ImGuiPanel("Viewport") {}
        virtual void View() override;
        virtual void PushStyle() override { ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); }
        virtual void PopStyle() override { ImGui::PopStyleVar(); }

        void SetFrameBuffer(Ref<FrameBuffer> buffer);
        uint32_t GetWidth() const { return m_ViewportSize.x; }
        uint32_t GetHeight() const { return m_ViewportSize.y; }

    private:
        ImVec2 m_ViewportSize;
        Ref<FrameBuffer> m_FrameBuffer;
    };
}  // namespace DE