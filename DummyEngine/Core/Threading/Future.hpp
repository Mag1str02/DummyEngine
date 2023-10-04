#pragma once

#include <DummyEngine/Utils/Base/STDIncludes.h>

#include "Task.hpp"

namespace DE {

    template <class T> class Future;

    template <class T> using FuturePtr = std::shared_ptr<Future<T>>;

    template <class T> class Future : public Task {
    public:
        template <class F> Future(const F& f) : f_(std::move(f)) {}
        virtual ~Future() {}
        virtual void Run() override {
            try {
                res_ = f_();
            } catch (std::exception& e) {
                e_ptr_ = std::current_exception();
            }
        }

        T Get() {
            Wait();
            if (e_ptr_) {
                std::rethrow_exception(e_ptr_);
            }
            return res_;
        }

    private:
        std::exception_ptr e_ptr_ = nullptr;
        T                  res_;
        std::function<T()> f_;
    };
}  // namespace DE