#pragma once

#include <DummyConcurrency/DummyConcurrency.hpp>  // IWYU pragma: export

namespace DummyEngine {

    class Error {
    public:
        Error() = default;
        explicit Error(const std::string& message) : message_(message) {}

        const std::string Message() const { return message_; }

    private:
        std::string message_;
    };

    namespace Futures {
        using NDummyConcurrency::NFuture::Submit;

        using namespace NDummyConcurrency::NFuture::NSyntax;
    }  // namespace Futures

    namespace Results {
        using namespace NDummyConcurrency::NResult::NSyntax;
    }

    namespace Fibers {
        using NDummyConcurrency::NFiber::Go;
        using NDummyConcurrency::NFiber::Yield;
    }  // namespace Fibers

    template <typename T>
    using TryFuture = NDummyConcurrency::Future<std::expected<T, Error>>;
    template <typename T>
    using Future = NDummyConcurrency::Future<T>;

    using NDummyConcurrency::NFiber::StackPool;
    using NDummyConcurrency::NRuntime::ExternalThreadPool;
    using NDummyConcurrency::NRuntime::FiberInvoker;
    using NDummyConcurrency::NRuntime::IScheduler;
    using NDummyConcurrency::NRuntime::RunLoop;
    using NDummyConcurrency::NRuntime::ThreadPool;

    using FMutex     = NDummyConcurrency::NSynchronization::NFiberAware::Mutex;
    using FEvent     = NDummyConcurrency::NSynchronization::NFiberAware::Event;
    using FWaitGroup = NDummyConcurrency::NSynchronization::NFiberAware::WaitGroup;

    using TSpinLock  = NDummyConcurrency::NSynchronization::NThread::SpinLock;
    using TWaitGroup = NDummyConcurrency::NSynchronization::NThread::WaitGroup;
    using TEvent     = NDummyConcurrency::NSynchronization::NThread::Event;
}  // namespace DummyEngine
