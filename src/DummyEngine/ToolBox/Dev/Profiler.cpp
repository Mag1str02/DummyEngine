#include "ToolBox/Dev/Profiler.h"
#include "Core/Rendering/Renderer/Renderer.h"

namespace DE
{
    ProfilerFrame::ProfilerFrame(uint32_t predicted_lapse_amount) { m_TimeLapses.reserve(predicted_lapse_amount); }

    Profiler::Profiler() : m_PrevFrameTimeLapseAmount(0) {}

    void Profiler::IBeginFrame()
    {
        if (!m_Frames.empty())
        {
            IPopTimeLapse();
        }
        m_Frames.push(ProfilerFrame(m_PrevFrameTimeLapseAmount));
        m_PrevFrameTimeLapseAmount = 0;

        // TODO: Move somewher frame storage size.
        if (m_Frames.size() > 2)
        {
            m_Frames.pop();
        }

        IPushTimeLapse("Frame");
    }

    void Profiler::IPushTimeLapse(const std::string& name)
    {
        ++m_PrevFrameTimeLapseAmount;
        uint32_t index = m_Frames.back().m_TimeLapses.size();
        m_Frames.back().m_TimeLapses.push_back(TimeLapse(name));
        m_Frames.back().m_TimeLapses.back().m_Start = std::chrono::high_resolution_clock::now();
        if (!m_TimeLapseStack.empty())
        {
            m_Frames.back().m_TimeLapses[m_TimeLapseStack.top()].m_Childs.push_back(index);
        }
        m_TimeLapseStack.push(index);
    }
    void Profiler::IPopTimeLapse()
    {
        DE_ASSERT(!m_TimeLapseStack.empty(), "Attempt to pop empty timeapse stack.");

        m_Frames.back().m_TimeLapses[m_TimeLapseStack.top()].m_End = std::chrono::high_resolution_clock::now();
        m_TimeLapseStack.pop();
    }

    Profiler& Profiler::Get()
    {
        static Profiler profiler;
        return profiler;
    }

    void Profiler::BeginFrame() { Get().IBeginFrame(); }
    const ProfilerFrame& Profiler::GetOldestFrame() { return Get().m_Frames.front(); }

    ProfilerScopeObject::ProfilerScopeObject(const std::string& name) { Profiler::Get().IPushTimeLapse(name); }
    ProfilerScopeObject::~ProfilerScopeObject() { Profiler::Get().IPopTimeLapse(); }

}  // namespace DE