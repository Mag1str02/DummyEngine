#include "Concurrency.h"

namespace DummyEngine {

    SINGLETON_BASE(Concurrency);
    S_INITIALIZE() {
        main_factory_.Construct("EngineThread", 1);
        background_factory_.Construct("BackgroundThread", 2);

        engine_main_thread_pool_.Construct(main_factory_.Get(), 1);
        background_thread_pool_.Construct(background_factory_.Get(), std::thread::hardware_concurrency());

        fiber_hint_provider_.Construct(&stack_pool_, "Background Fiber", 2);
        fiber_invoker_.Construct(fiber_hint_provider_.Get(), background_thread_pool_.Get());
        background_thread_pool_->SetInvoker(fiber_invoker_.Get());
        return Unit();
    }
    S_TERMINATE() {
        fiber_invoker_.Destruct();
        fiber_hint_provider_.Destruct();

        engine_main_thread_pool_.Destruct();
        background_thread_pool_.Destruct();

        main_factory_.Destruct();
        background_factory_.Destruct();

        return Unit();
    }

    S_METHOD_IMPL(Unit, Start, (), ()) {
        engine_main_thread_pool_->Start();
        background_thread_pool_->Start();
        return Unit();
    }
    S_METHOD_IMPL(Unit, Stop, (), ()) {
        background_thread_pool_->Stop();
        engine_main_thread_pool_->Stop();
        return Unit();
    }

    S_METHOD_IMPL(IScheduler&, GetEngineMainScheduler, (), ()) {
        return *(engine_main_thread_pool_.Get());
    }
    S_METHOD_IMPL(IScheduler&, GetEngineBackgroundScheduler, (), ()) {
        return *(background_thread_pool_.Get());
    }

    S_METHOD_IMPL(ExternalThreadPool&, GetMainThreadScheduler, (), ()) {
        return main_thread_scheduler_;
    }

}  // namespace DummyEngine
