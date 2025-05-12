#pragma once

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

#include <chrono>
#include <queue>
#include <stack>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>

namespace DummyEngine {

    struct TimeLapse {
        std::chrono::time_point<std::chrono::high_resolution_clock> Start;
        std::chrono::time_point<std::chrono::high_resolution_clock> End;
        std::vector<U32>                                            Childs;
        std::string                                                 Name;

        float       Duration() const { return (End - Start).count() * 0.001 * 0.001; }
        std::string StrDuration() const { return std::to_string((End - Start).count() * 0.001 * 0.001) + "ms"; }

        explicit TimeLapse(const std::string& name) : Name(name) {}
    };
    struct ProfilerFrame {
        std::vector<TimeLapse> Timelapses;

        explicit ProfilerFrame(U32 predicted_lapse_amount);
    };

    class Profiler : public Singleton<Profiler> {
        SINGLETON(Profiler)
    public:
        S_METHOD_DEF(const ProfilerFrame&, GetOldestFrame, ());
        S_METHOD_DEF(Unit, BeginFrame, ());
        S_METHOD_DEF(Unit, PushTimeLapse, (const std::string& name));
        S_METHOD_DEF(Unit, PopTimeLapse, ());

    private:
        friend class ProfilerScopeObject;

        std::queue<ProfilerFrame> frames_;
        std::stack<U32>           timelapse_stack_;
        U32                       prev_frame_timelapse_amount_ = 0;
    };

    class ProfilerScopeObject {
    public:
        explicit ProfilerScopeObject(const std::string& name);
        ~ProfilerScopeObject();
    };

}  // namespace DummyEngine

#if DE_ENABLE_PROFILER
#define DE_PROFILE_SCOPE(name) ZoneScopedN(name)
#define DE_PROFILER_BEGIN_FRAME() \
    do {                          \
        FrameMark;                \
    } while (false)

#else
#define DE_PROFILE_SCOPE(name)
#define DE_PROFILER_BEGIN_FRAME()
#endif
