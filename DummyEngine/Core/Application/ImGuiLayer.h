#pragma once

#include "DummyEngine/Core/Application/Layer.h"

namespace DummyEngine {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void BeginFrame();
        void EndFrame();
    };

}  // namespace DummyEngine
