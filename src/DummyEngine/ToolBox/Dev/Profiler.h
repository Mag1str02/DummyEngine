#pragma once

#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <iostream>
#include "DummyEngine/Utils/Singleton.h"

namespace DE
{
    struct TimeLapse
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_End;
        std::vector<uint32_t>                                       m_Childs;
        std::string                                                 m_Name;

        float       Duration() const { return (m_End - m_Start).count() * 0.001 * 0.001; }
        std::string StrDuration() const { return std::to_string((m_End - m_Start).count() * 0.001 * 0.001) + "ms"; }

        TimeLapse(const std::string& name) : m_Name(name) {}
    };
    struct ProfilerFrame
    {
        std::vector<TimeLapse> m_TimeLapses;

        ProfilerFrame(uint32_t predicted_lapse_amount);
    };

    class Profiler : public Singleton<Profiler>
    {
        SINGLETON(Profiler)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(const ProfilerFrame&, GetOldestFrame, ());
        S_METHOD_DEF(Unit, BeginFrame, ());
        S_METHOD_DEF(Unit, PushTimeLapse, (const std::string& name));
        S_METHOD_DEF(Unit, PopTimeLapse, ());

    private:
        friend class ProfilerScopeObject;
        Profiler()  = default;
        ~Profiler() = default;

        std::queue<ProfilerFrame> m_Frames;
        std::stack<uint32_t>      m_TimeLapseStack;
        uint32_t                  m_PrevFrameTimeLapseAmount = 0;
    };

    class ProfilerScopeObject
    {
    public:
        ProfilerScopeObject(const std::string& name);
        ~ProfilerScopeObject();
    };

#if DE_PROFILER_ENABLED
#define DE_PROFILE_SCOPE(name) ProfilerScopeObject profiler_scope_object(name)
#define DE_PROFILER_BEGIN_FRAME() Profiler::BeginFrame()
#else
#define DE_PROFILE_SCOPE(name)
#define DE_PROFILER_BEGIN_FRAME()
#endif

}  // namespace DE