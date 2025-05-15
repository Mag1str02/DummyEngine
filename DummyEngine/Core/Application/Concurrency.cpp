#include "Concurrency.h"

namespace DummyEngine {

    SINGLETON_BASE(Concurrency);
    S_INITIALIZE() {
        fiber_hint_provider_.Construct(&stack_pool_, "Background Fiber", 2);
        fiber_invoker_.Construct(fiber_hint_provider_.Get(), &back_ground_thread_pool_);
        back_ground_thread_pool_.SetInvoker(fiber_invoker_.Get());
        return Unit();
    }
    S_TERMINATE() {
        fiber_invoker_.Destruct();
        fiber_hint_provider_.Destruct();
        return Unit();
    }

    S_METHOD_IMPL(Unit, Start, (), ()) {
        engine_main_thread_pool_.Start();
        back_ground_thread_pool_.Start();
        return Unit();
    }
    S_METHOD_IMPL(Unit, Stop, (), ()) {
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
