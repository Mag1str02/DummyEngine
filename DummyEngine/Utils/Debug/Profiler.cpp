#include "DummyEngine/Utils/Debug/Profiler.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {
    ProfilerFrame::ProfilerFrame(uint32_t predicted_lapse_amount) {
        m_TimeLapses.reserve(predicted_lapse_amount);
    }

    SINGLETON_BASE(Profiler);
    S_INITIALIZE() {
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(const ProfilerFrame&, GetOldestFrame, (), ()) {
        return Get().m_Frames.front();
    }
    S_METHOD_IMPL(Unit, BeginFrame, (), ()) {
        if (!m_Frames.empty()) {
            IPopTimeLapse();
        }
        m_Frames.push(ProfilerFrame(m_PrevFrameTimeLapseAmount));
        m_PrevFrameTimeLapseAmount = 0;

        // TODO: Move somewher frame storage size.
        if (m_Frames.size() > 2) {
            m_Frames.pop();
        }

        IPushTimeLapse("Frame");
        return Unit();
    }
    S_METHOD_IMPL(Unit, PushTimeLapse, (const std::string& name), (name)) {
        ++m_PrevFrameTimeLapseAmount;
        uint32_t index = m_Frames.back().m_TimeLapses.size();
        m_Frames.back().m_TimeLapses.push_back(TimeLapse(name));
        m_Frames.back().m_TimeLapses.back().m_Start = std::chrono::high_resolution_clock::now();
        if (!m_TimeLapseStack.empty()) {
            m_Frames.back().m_TimeLapses[m_TimeLapseStack.top()].m_Childs.push_back(index);
        }
        m_TimeLapseStack.push(index);
        return Unit();
    }
    S_METHOD_IMPL(Unit, PopTimeLapse, (), ()) {
        DE_ASSERT(!m_TimeLapseStack.empty(), "Attempt to pop empty timelapse stack.");

        m_Frames.back().m_TimeLapses[m_TimeLapseStack.top()].m_End = std::chrono::high_resolution_clock::now();
        m_TimeLapseStack.pop();
        return Unit();
    }

    ProfilerScopeObject::ProfilerScopeObject(const std::string& name) {
        Profiler::PushTimeLapse(name);
    }
    ProfilerScopeObject::~ProfilerScopeObject() {
        Profiler::PopTimeLapse();
    }

}  // namespace DE