#pragma once

#include "DummyEngine/Core/Application/Layer.h"

namespace DE {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void BeginFrame();
        void EndFrame();

    private:
        void SetDarkThemeColors();
    };

}  // namespace DE
