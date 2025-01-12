#pragma once

#include <chrono>

namespace DummyEngine {

    class Timer {
    public:
        Timer();

        float Time() const;
        float TimeM() const;
        float Reset();

    private:
        std::chrono::system_clock::time_point start_time_;
    };

}  // namespace DummyEngine