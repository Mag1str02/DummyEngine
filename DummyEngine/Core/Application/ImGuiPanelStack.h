#pragma once

#include "DummyEngine/Core/Application/ImGuiPanel.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class ImGuiPanelStack {
    public:
        ImGuiPanelStack() = default;

        void PushPanel(ImGuiPanel* panel);
        void OnImGuiRender();

        std::vector<Pair<const std::string&, bool*>> GetControllers();

    private:
        std::vector<ImGuiPanel*> m_Panels;
    };
}  // namespace DE