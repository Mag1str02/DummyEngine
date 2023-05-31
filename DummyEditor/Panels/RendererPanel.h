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

        float m_Exposure = 1;
        float m_Gamma    = 1;
        bool  m_GammaHDR = true;
    };
}  // namespace DE