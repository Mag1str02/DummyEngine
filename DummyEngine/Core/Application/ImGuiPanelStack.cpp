#include "DummyEngine/Core/Application/ImGuiPanelStack.h"

namespace DE
{
    void ImGuiPanelStack::PushPanel(ImGuiPanel* panel) { m_Panels.push_back(panel); }
    void ImGuiPanelStack::OnImGuiRender()
    {
        for (auto panel : m_Panels)
        {
            panel->OnImGuiRender();
        }
    }

    std::vector<Pair<const std::string&, bool*>> ImGuiPanelStack::GetControllers()
    {
        std::vector<Pair<const std::string&, bool*>> res;
        res.reserve(m_Panels.size());
        for (auto panel : m_Panels)
        {
            res.push_back({panel->GetName(), panel->GetController()});
        }
        return res;
    }

}  // namespace DE