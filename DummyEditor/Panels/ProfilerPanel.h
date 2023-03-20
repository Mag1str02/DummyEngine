#pragma once

#include "DummyEditor/DummyEngineInclude.h"

namespace DE {
    class ProfilerPanel : public ImGuiPanel {
    public:
        ProfilerPanel() : ImGuiPanel("Profiler") {}
        virtual void View() override;

    private:
        void RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, uint32_t index);
    };
}  // namespace DE