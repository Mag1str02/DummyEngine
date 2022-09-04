#include "Panels/ProfilerPanel.h"

namespace DE
{
    ProfilerPanel::ProfilerPanel() {}

    void ProfilerPanel::OnImGuiRender()
    {
        ImGui::Begin("Profiler");
        RenderTimeLapse(Profiler::GetOldestFrame().m_TimeLapses, 0);
        ImGui::End();
    }

    void ProfilerPanel::RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, uint32_t index)
    {
        std::string node_name = (time_lapses[index].m_Name);
        if (ImGui::TreeNode(node_name.c_str()))
        {
            ImGui::Text(time_lapses[index].StrDuration().c_str());
            for (auto child : time_lapses[index].m_Childs)
            {
                RenderTimeLapse(time_lapses, child);
            }
            ImGui::TreePop();
        }
    }
}  // namespace DE