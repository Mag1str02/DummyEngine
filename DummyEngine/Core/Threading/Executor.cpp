//
// Created by balanda on 10/3/2023.
//

#include "Executor.hpp"
namespace DE {

    void Executor::Submit(std::shared_ptr<Task> task) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (task->IsCanceled() || state_ >= ExecutorState::ShutDown) {
            return;
        }
        if (task->HasTimeTrigger()) {
            lock.unlock();
            timer_.AddTask(task);
            lock.lock();
        }
        task_available_ = true;
        tasks_.push_back(task);
        task->SubmitTo(this);
        wait_for_task_.notify_one();
    }

    std::shared_ptr<Task> Executor::WaitForTask() {
        std::unique_lock<std::mutex> lock(mutex_);
        std::shared_ptr<Task>        res = nullptr;
        while (true) {
            wait_for_task_.wait(lock, [this]() { return this->task_available_ || this->state_ == ExecutorState::Finished; });
            bool was_not_finished = false;
            for (int64_t i = 0; i < static_cast<int64_t>(tasks_.size()); ++i) {
                if (!was_not_finished && tasks_[i]->IsFinished()) {
                    tasks_.pop_front();
                    --i;
                    continue;
                }
                was_not_finished = true;
                if (tasks_[i]->IsReadyToExecute()) {
                    res = tasks_[i];
                    res->BeginProcessing();
                    break;
                }
            }
            if (!res) {
                if (state_ == ExecutorState::Finished) {
                    break;
                }
                task_available_ = false;
                continue;
            } else {
                break;
            }
        }
        lock.unlock();
        wait_for_task_.notify_one();
        return res;
    }

    Executor::Executor(size_t num_threads) {
        std::unique_lock<std::mutex> lock(mutex_);
        threads_.reserve(num_threads + 1);
        for (size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back(Worker, i, this);
        }
        threads_.emplace_back(TimerWorker, &timer_);
        state_ = ExecutorState::Working;
    }
    Executor::~Executor() {
        if (state_ != ExecutorState::Finished) {
            WaitShutdown();
        }
    }

    void Executor::StartShutdown() {
        std::unique_lock<std::mutex> lock(mutex_);
        state_ = ExecutorState::ShutDown;
    }
    void Executor::WaitShutdown() {
        mutex_.lock();
        state_ = ExecutorState::Finished;
        mutex_.unlock();
        wait_for_task_.notify_all();
        for (size_t i = 0; i < threads_.size() - 1; ++i) {
            threads_[i].join();
        }
        for (size_t i = 0; i < tasks_.size(); ++i) {
            tasks_[i]->ResetExecutor();
        }
        timer_.Shutdown();
        threads_.back().join();
        tasks_.clear();
    }
    void Executor::NotifyAvailable() {
        std::unique_lock<std::mutex> lock(mutex_);
        task_available_ = true;
        wait_for_task_.notify_one();
    }

    std::shared_ptr<Executor> MakeThreadPoolExecutor(int num_threads) {
        return std::make_shared<Executor>(num_threads);
    }

}  // namespace DE