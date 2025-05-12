#pragma once

#include "DummyEngine/Core/Application/Layer.h"

namespace DummyEngine {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(const Event& event) override;

        void BeginFrame();
        void EndFrame();

    private:
        bool mouse_locked_ = false;
    };

}  // namespace DummyEngine
