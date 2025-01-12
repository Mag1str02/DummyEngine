#include "Timer.h"

namespace DummyEngine {

    Timer::Timer() {
        start_time_ = std::chrono::system_clock::now();
    }

    float Timer::Time() const {
        return (std::chrono::system_clock::now() - start_time_).count();
    }
    float Timer::TimeM() const {
        return (std::chrono::system_clock::now() - start_time_).count() * 1000;
    }
    float Timer::Reset() {
        float res   = (std::chrono::system_clock::now() - start_time_).count();
        start_time_ = std::chrono::system_clock::now();
        return res;
    }

}  // namespace DummyEngine