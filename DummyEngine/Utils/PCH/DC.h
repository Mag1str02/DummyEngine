#pragma once

#include <DummyConcurrency/DummyConcurrency.hpp>  // IWYU pragma: export

namespace DummyEngine {

    class Error {
    public:
        Error() = default;
        Error(const std::string& message) : message_(message) {}  // NOLINT
        Error(const char* message) : message_(message) {}         // NOLINT

        const std::string Message() const { return message_; }

    private:
        std::string message_;
    };

    namespace Futures {
        using NDummyConcurrency::NFuture::Go;
        using NDummyConcurrency::NFuture::Submit;

        using NDummyConcurrency::NFuture::Ok;
        using NDummyConcurrency::NFuture::Just;

        using namespace NDummyConcurrency::NFuture::NSyntax;

        inline auto Failure() {
            return NDummyConcurrency::NFuture::Failure(std::error_code());
        }

        template <typename T>
        std::vector<T> WaitAll(std::vector<NDummyConcurrency::Future<T>>&& futures) {
            std::vector<T> results;
            results.reserve(futures.size());
            for (auto& future : futures) {
                results.emplace_back(std::move(future) | Get());
            }
            return results;
        }
    }  // namespace Futures

    namespace Results {
        using namespace NDummyConcurrency::NResult::NSyntax;

        using NDummyConcurrency::NResult::Ok;
        using NDummyConcurrency::NResult::Status;

        inline auto Failure() {
            return NDummyConcurrency::NResult::Failure(std::error_code());
        }
    }  // namespace Results

    namespace Fibers {
        using NDummyConcurrency::NFiber::Go;
        using NDummyConcurrency::NFiber::Yield;

    }  // namespace Fibers

    template <typename T>
    using Result = NDummyConcurrency::Result<T>;
    template <typename T>
    using Future = NDummyConcurrency::Future<T>;
    template <typename T>
    using TryFuture = Future<Result<T>>;
    template <typename T>
    using CopyFuture = NDummyConcurrency::NFuture::CopyFuture<T>;
    template <typename T>
    using CopyTryFuture = NDummyConcurrency::NFuture::CopyFuture<Result<T>>;
    template <typename T>
    using ManualLifetime = NDummyConcurrency::ManualLifetime<T>;

    using NDummyConcurrency::NFiber::Hint;
    using NDummyConcurrency::NFiber::InlineStackPool;
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
