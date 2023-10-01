#pragma once
#include "DummyEngine/Core/Application/Layer.h"

namespace DE {
    class ConsoleLayer : public Layer {
        void OnAttach() override;
        void OnUpdate(float dt) override;
        void OnImGuiRender() override;
        void OnDetach() override;

    private:
        bool        m_Show = false;
        std::string m_Command;
        int      m_historyPosition = 0;
        bool        m_ScrollToBottom  = true;
    };
}  // namespace DE