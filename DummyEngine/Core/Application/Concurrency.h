#pragma once

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/PCH/DC.h"

namespace DummyEngine {

    class Concurrency : public Singleton<Concurrency> {
        SINGLETON(Concurrency)
    public:
        S_METHOD_DEF(IScheduler&, GetEngineMainScheduler, ());
        S_METHOD_DEF(IScheduler&, GetEngineBackgroundScheduler, ());
        S_METHOD_DEF(ExternalThreadPool&, GetMainThreadScheduler, ());

    private:
        ExternalThreadPool           main_thread_scheduler_;
        StackPool                    stack_pool_;
        ManualLifetime<FiberInvoker> fiber_invoker_;
        ThreadPool                   engine_main_thread_pool_ = ThreadPool(1);
        ThreadPool                   back_ground_thread_pool_ = ThreadPool(std::thread::hardware_concurrency() * 2);
    };
}  // namespace DummyEngine