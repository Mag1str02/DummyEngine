#include "Concurrency.h"

namespace DummyEngine {

    SINGLETON_BASE(Concurrency);
    S_INITIALIZE() {
        engine_main_thread_pool_.Start();
        back_ground_thread_pool_.Start();
        return Unit();
    }
    S_TERMINATE() {
        back_ground_thread_pool_.Stop();
        engine_main_thread_pool_.Stop();
        return Unit();
    }

    S_METHOD_IMPL(IScheduler&, GetEngineMainScheduler, (), ()) {
        return engine_main_thread_pool_;
    }
    S_METHOD_IMPL(IScheduler&, GetEngineBackgroundScheduler, (), ()) {
        return back_ground_thread_pool_;
    }

    S_METHOD_IMPL(ExternalThreadPool&, GetMainThreadScheduler, (), ()) {
        return main_thread_scheduler_;
    }

}  // namespace DummyEngine
