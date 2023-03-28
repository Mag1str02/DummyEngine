#include "DummyEngine/Utils/Helpers/Timer.h"

namespace DE {
    Timer::Timer() {
        m_StartTime = std::chrono::system_clock::now();
    }

    float Timer::Time() const {
        return (std::chrono::system_clock::now() - m_StartTime).count();
    }
    float Timer::TimeM() const {
        return (std::chrono::system_clock::now() - m_StartTime).count() * 1000;
    }
    float Timer::Reset() {
        float res   = (std::chrono::system_clock::now() - m_StartTime).count();
        m_StartTime = std::chrono::system_clock::now();
        return res;
    }
}  // namespace DE