#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Application/ImGuiPanel.h"

namespace DE
{
    class ImGuiPanelStack
    {
    public:
        ImGuiPanelStack() = default;

        void PushPanel(ImGuiPanel* panel);
        void OnImGuiRender();

        std::vector<Pair<const std::string&, bool*>> GetControllers();

    private:
        std::vector<ImGuiPanel*> m_Panels;
    };
}  // namespace DE