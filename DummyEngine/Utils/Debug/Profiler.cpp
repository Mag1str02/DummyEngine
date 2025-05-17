#include "Profiler.h"

#include <tracy/Tracy.hpp>

namespace DummyEngine {

    ProfilerFrame::ProfilerFrame(U32 predicted_lapse_amount) {
        Timelapses.reserve(predicted_lapse_amount);
    }

    SINGLETON_BASE(Profiler);
    S_INITIALIZE() {
        tracy::StartupProfiler();
        return Unit();
    }
    S_TERMINATE() {
        tracy::ShutdownProfiler();
        return Unit();
    }

    S_METHOD_IMPL(const ProfilerFrame&, GetOldestFrame, (), ()) {
        return frames_.front();
    }
    S_METHOD_IMPL(Unit, BeginFrame, (), ()) {
        if (!frames_.empty()) {
            IPopTimeLapse();
        }
        frames_.push(ProfilerFrame(prev_frame_timelapse_amount_));
        prev_frame_timelapse_amount_ = 0;

        // TODO: Move somewhere frame storage size.
        if (frames_.size() > 2) {
            frames_.pop();
        }

        IPushTimeLapse("Frame");
        return Unit();
    }
    S_METHOD_IMPL(Unit, PushTimeLapse, (const std::string& name), (name)) {
        ++prev_frame_timelapse_amount_;
        U32 index = frames_.back().Timelapses.size();
        frames_.back().Timelapses.push_back(TimeLapse(name));
        frames_.back().Timelapses.back().Start = std::chrono::high_resolution_clock::now();
        if (!timelapse_stack_.empty()) {
            frames_.back().Timelapses[timelapse_stack_.top()].Childs.push_back(index);
        }
        timelapse_stack_.push(index);
        return Unit();
    }
    S_METHOD_IMPL(Unit, PopTimeLapse, (), ()) {
        DE_ASSERT(!timelapse_stack_.empty(), "Attempt to pop empty timelapse stack.");

        frames_.back().Timelapses[timelapse_stack_.top()].End = std::chrono::high_resolution_clock::now();
        timelapse_stack_.pop();
        return Unit();
    }

    ProfilerScopeObject::ProfilerScopeObject(const std::string& name) {
        Profiler::PushTimeLapse(name);
    }
    ProfilerScopeObject::~ProfilerScopeObject() {
        Profiler::PopTimeLapse();
    }

    ThreadFactory::ThreadFactory(const std::string& base_name, int32_t group) : base_name_(base_name), group_(group) {}
    std::thread ThreadFactory::LaunchThread(std::function<void()> main) {
        names_.emplace_back(std::format("{} ({})\0", base_name_, names_.size()));
        const char* name  = names_.back().c_str();
        auto        group = group_;
        return std::thread([main = std::move(main), name, group]() {
            NDummyConcurrency::NImplementationLayer::Fiber fiber;
            fiber.SetName(name, group);
            NDummyConcurrency::NImplementationLayer::SwitchToFiber(fiber.Handle());
            {
                DE_PROFILE_SCOPE("Worker Thread Main");
                main();
            }
            NDummyConcurrency::NImplementationLayer::SwitchToFiber(NDummyConcurrency::NImplementationLayer::FiberHandle());
        });
    }

}  // namespace DummyEngine