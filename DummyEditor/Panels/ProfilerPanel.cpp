#include "DummyEditor/Panels/ProfilerPanel.h"

namespace DE {
    void ProfilerPanel::OnImGui() {
        DE_PROFILE_SCOPE("ProfilerPanel OnImGui");

        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_SPEED "  Profiler")) {
#if DE_ENABLE_PROFILER
                RenderTimeLapse(Profiler::GetOldestFrame().m_TimeLapses, 0);
#endif
            }
            ImGui::End();
        }
    }

    void ProfilerPanel::RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, U32 index) {
        ImGuiUtils::ScopedID id(index);
        ImGuiTreeNodeFlags   flags =
            ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        auto& name     = time_lapses[index].m_Name;
        auto  duration = time_lapses[index].StrDuration();
        if (time_lapses[index].m_Childs.empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        }
        if (ImGui::TreeNodeEx(name.c_str(), flags, "%s: %s", name.c_str(), duration.c_str())) {
            for (auto child : time_lapses[index].m_Childs) {
                RenderTimeLapse(time_lapses, child);
            }
            ImGui::TreePop();
        }
    }
}  // namespace DE