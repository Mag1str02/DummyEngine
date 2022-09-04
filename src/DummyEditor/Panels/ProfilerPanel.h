#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class ProfilerPanel
    {
    public:
        ProfilerPanel();
        void OnImGuiRender();

    private:
        void RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, uint32_t index);
    };
}  // namespace DE