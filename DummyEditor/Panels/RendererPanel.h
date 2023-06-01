#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class RendererPanel : public ImGuiPanel {
    public:
        RendererPanel() : ImGuiPanel("Renderer") {}
        virtual void OnImGui() override;

        void SetScene(Ref<Scene> scene);

    private:
        WeakRef<Scene> m_Scene;

        bool  m_GammaHDR = false;
        float m_Exposure = 1;
        float m_Gamma    = 1;

        bool  m_Bloom             = false;
        float m_BloomTreshold     = 1;
        float m_BloomSoftTreshold = 0;
        float m_BloomStrength     = 0.04;
        float m_BloomRadius       = 0.005;
        U32   m_BloomDepth        = 1;
    };
}  // namespace DE