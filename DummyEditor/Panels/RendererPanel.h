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
    };
}  // namespace DE