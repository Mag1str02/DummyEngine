#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class Scene;

    class RendererPanel : public ImGuiPanel {
    public:
        RendererPanel() : ImGuiPanel("Renderer") {}
        virtual void OnImGui() override;

        void SetScene(Ref<Scene> scene);

    private:
        WeakRef<Scene> scene_;
    };

}  // namespace DummyEngine