#pragma once

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/PCH/DC.h"

namespace DummyEngine {

    class Concurrency : public Singleton<Concurrency> {
        SINGLETON(Concurrency)
    public:
        S_METHOD_DEF(ThreadPool&, GetEngineMainScheduler, ());
        S_METHOD_DEF(ExternalThreadPool&, GetMainThreadScheduler, ());

    private:
        ExternalThreadPool main_thread_scheduler_;
        ThreadPool         engine_main_thread_pool_ = ThreadPool(1);
    };
}  // namespace DummyEngine