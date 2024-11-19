#pragma once

#include <chrono>
#include <queue>
#include <stack>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {
    struct TimeLapse {
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_End;
        std::vector<U32>                                            m_Childs;
        std::string                                                 m_Name;

        float       Duration() const { return (m_End - m_Start).count() * 0.001 * 0.001; }
        std::string StrDuration() const { return std::to_string((m_End - m_Start).count() * 0.001 * 0.001) + "ms"; }

        TimeLapse(const std::string& name) : m_Name(name) {}
    };
    struct ProfilerFrame {
        std::vector<TimeLapse> m_TimeLapses;

        ProfilerFrame(U32 predicted_lapse_amount);
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

        std::queue<ProfilerFrame> m_Frames;
        std::stack<U32>           m_TimeLapseStack;
        U32                       m_PrevFrameTimeLapseAmount = 0;
    };

    class ProfilerScopeObject {
    public:
        ProfilerScopeObject(const std::string& name);
        ~ProfilerScopeObject();
    };

#if DE_ENABLE_PROFILER
#define DE_PROFILE_SCOPE(name)                       \
    ProfilerScopeObject profiler_scope_object(name); \
    ZoneScopedN(name)
#define DE_PROFILER_BEGIN_FRAME() \
    do {                          \
        Profiler::BeginFrame();   \
        FrameMark;                \
    } while (false)

#else
#define DE_PROFILE_SCOPE(name)
#define DE_PROFILER_BEGIN_FRAME()
#endif

}  // namespace DE