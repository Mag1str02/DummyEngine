#pragma once

#include "Addition/Base.h"
#include "Core/Application/ImGuiPanel.h"

namespace DE
{
    class ImGuiPanelStack
    {
    public:
        ImGuiPanelStack();

        void PushPanel(ImGuiPanel* panel);
        void OnImGuiRender();

        std::vector<Pair<const std::string&, bool*>> GetControllers();

    private:
        std::vector<ImGuiPanel*> m_Panels;
    };
}  // namespace DE