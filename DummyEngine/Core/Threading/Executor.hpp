#pragma once


#include <DummyEngine/Utils/Base/STDIncludes.h>
#include "Future.hpp"
#include "Task.hpp"
#include "TimerQueue.hpp"

namespace DE {


    class Executor
    {
    public:
        Executor(size_t num_threads = std::thread::hardware_concurrency());
        ~Executor();

        void Submit(std::shared_ptr<Task> task);

        void StartShutdown();
        void WaitShutdown();

        template <class T> FuturePtr<T> Invoke(std::function<T()> fn)
        {
            FuturePtr<T> task = std::make_shared<Future<T>>(fn);
            Submit(task);
            return task;
        }

        template <class Y, class T> FuturePtr<Y> Then(FuturePtr<T> input, std::function<Y()> fn)
        {
            FuturePtr<Y> task = std::make_shared<Future<Y>>(fn);
            task->AddDependency(input);
            Submit(task);
            return task;
        }

        template <class T> FuturePtr<std::vector<T>> WhenAll(std::vector<FuturePtr<T>> all)
        {
            auto func = [all]()
            {
                std::vector<T> ans;
                for (auto t : all)
                {
                    ans.push_back(t->Get());
                }
                return ans;
            };
            FuturePtr<std::vector<T>> task = std::make_shared<Future<std::vector<T>>>(func);
            for (auto t : all)
            {
                task->AddDependency(t);
            }
            Submit(task);
            return task;
        }

        template <class T> FuturePtr<std::vector<T>> WhenAllBeforeDeadline(std::vector<FuturePtr<T>> all, std::chrono::system_clock::time_point deadline)
        {
            auto func = [all]()
            {
                std::vector<T> ans;
                for (auto t : all)
                {
                    if (t->IsFinished())
                    {
                        ans.push_back(t->Get());
                    }
                }
                return ans;
            };
            FuturePtr<std::vector<T>> task = std::make_shared<Future<std::vector<T>>>(func);
            task->SetTimeTrigger(deadline);
            Submit(task);
            return task;
        }

    private:
        friend void Worker(size_t worker_id, Executor* executor);
        friend class Task;
        void                  NotifyAvailable();
        std::shared_ptr<Task> WaitForTask();

        TimerQueue                        timer_;
        ExecutorState                     state_;
        std::deque<std::shared_ptr<Task>> tasks_;
        bool                              task_available_ = false;

        std::vector<std::thread> threads_;

        std::mutex              mutex_;
        std::condition_variable wait_for_task_;
    };

    std::shared_ptr<Executor> MakeThreadPoolExecutor(int num_threads);
};