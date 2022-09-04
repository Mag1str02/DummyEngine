#pragma once

#include <chrono>

namespace DE
{
    class Timer
    {
    public:
        Timer();

        float Time() const;
        float TimeM() const;
        float Reset();

    private:
        std::chrono::_V2::system_clock::time_point m_StartTime;
    };
}  // namespace DE