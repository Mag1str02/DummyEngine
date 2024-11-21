#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

#include "DummyEngine/Utils/Debug/Profiler.h"

namespace DummyEngine {

    class ProfilerPanel : public ImGuiPanel {
    public:
        ProfilerPanel() : ImGuiPanel("Profiler") {}
        virtual void OnImGui() override;

    private:
        void RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, U32 index);
    };

}  // namespace DummyEngine