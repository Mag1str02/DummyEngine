#include "DummyEditor/Panels/ProfilerPanel.h"

namespace DE {
    void ProfilerPanel::View() {
        DE_PROFILE_SCOPE("ProfilerPanel OnImGuiRender");
#if DE_ENABLE_PROFILER
        RenderTimeLapse(Profiler::GetOldestFrame().m_TimeLapses, 0);
#endif
    }

    void ProfilerPanel::RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, U32 index) {
        std::string node_name = (time_lapses[index].m_Name);
        if (time_lapses[index].m_Childs.empty()) {
            ImGui::BulletText("%s: %s", node_name.c_str(), time_lapses[index].StrDuration().c_str());
        } else {
            if (ImGui::TreeNode(this + index, "%s: %s", node_name.c_str(), time_lapses[index].StrDuration().c_str())) {
                for (auto child : time_lapses[index].m_Childs) {
                    RenderTimeLapse(time_lapses, child);
                }
                ImGui::TreePop();
            }
        }
    }
}  // namespace DE