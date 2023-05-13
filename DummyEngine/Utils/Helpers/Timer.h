#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"

namespace DE {
    class Timer {
    public:
        Timer();

        float Time() const;
        float TimeM() const;
        float Reset();

    private:
        std::chrono::system_clock::time_point m_StartTime;
    };
}  // namespace DE