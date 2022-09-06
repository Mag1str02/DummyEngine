#include "DummyEngineInclude.h"

namespace DE
{
    class ViewportPanel
    {
    public:
        ViewportPanel();

        void OnImGuiRender(bool& enabled);
        void SetFrameBuffer(Ref<FrameBuffer> buffer);

        uint32_t GetWidth() const { return m_ViewportSize.x; }
        uint32_t GetHeight() const { return m_ViewportSize.y; }

    private:
        ImVec2 m_ViewportSize;
        Ref<FrameBuffer> m_FrameBuffer;
    };
}  // namespace DE