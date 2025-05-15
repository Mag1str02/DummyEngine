#pragma once

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/PCH/DC.h"

namespace DummyEngine {

    class Concurrency : public Singleton<Concurrency> {
        SINGLETON(Concurrency)
    public:
        S_METHOD_DEF(Unit, Start, ());
        S_METHOD_DEF(Unit, Stop, ());

        S_METHOD_DEF(IScheduler&, GetEngineMainScheduler, ());
        S_METHOD_DEF(IScheduler&, GetEngineBackgroundScheduler, ());
        S_METHOD_DEF(ExternalThreadPool&, GetMainThreadScheduler, ());

    private:
        ExternalThreadPool                     main_thread_scheduler_;
        StackPool                              stack_pool_;
        ManualLifetime<FiberGroupHintProvider> fiber_hint_provider_;
        ManualLifetime<FiberInvoker>           fiber_invoker_;

        ManualLifetime<ThreadFactory> main_factory_;
        ManualLifetime<ThreadFactory> background_factory_;

        ManualLifetime<ThreadPool> engine_main_thread_pool_;
        ManualLifetime<ThreadPool> background_thread_pool_;
    };
}  // namespace DummyEngine