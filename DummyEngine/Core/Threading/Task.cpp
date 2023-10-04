#include "Task.hpp"

#include <utility>
#include "Executor.hpp"

namespace DE {

    void Task::AddDependency(const std::shared_ptr<Task>& dep)
    {
        std::unique_lock<std::mutex> lock(dep->mutex_);
        if (dep->IsFinished())
        {
            return;
        }
        dep->sub_dependency_.push_back(weak_from_this());
        has_dependency_ = true;
        no_condition_   = false;
        ++dependency_count_;
    }
    void Task::AddTrigger(const std::shared_ptr<Task>& dep)
    {
        dep->sub_trigger_.push_back(shared_from_this());
        has_trigger_  = true;
        no_condition_ = false;
    }
    void Task::SetTimeTrigger(TimePoint at)
    {
        execute_after_ = at;
        has_timer_     = true;
        no_condition_  = false;
    }

    bool Task::IsReadyToExecute()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        bool                         processing = status_.load() >= TaskStatus::Processing;
        if (processing)
        {
            return false;
        }
        if (no_condition_)
        {
            return true;
        }
        if (has_dependency_ && dependency_count_ == finished_dependency_count_.load())
        {
            return true;
        }
        if (has_trigger_ && triggered_.load())
        {
            return true;
        }
        if (has_timer_ && the_time_has_come_.load())
        {
            return true;
        }
        return false;
    }
    bool Task::IsCompleted() { return status_.load() == TaskStatus::Completed; }
    bool Task::IsFailed() { return status_.load() == TaskStatus::Failed; }
    bool Task::IsCanceled() { return status_.load() == TaskStatus::Canceled; }
    bool Task::IsFinished() { return status_.load() >= TaskStatus::Canceled; }
    bool Task::HasTimeTrigger() const { return has_timer_; }

    std::exception_ptr Task::GetError()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return e_ptr_;
    }

    void Task::Finish(TaskStatus status, std::exception_ptr exception)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        status_.store(status);
        lock.unlock();
        for (const auto& sub_dep : sub_dependency_)
        {
            auto t = sub_dep.lock();
            if (t)
            {
                t->finished_dependency_count_.fetch_add(1);
            }
        }
        for (const auto& sub_trg : sub_trigger_)
        {
            sub_trg->triggered_.store(true);
        }
        UpdateReadyStatus();
        ResetExecutor();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (status == TaskStatus::Failed)
            {
                e_ptr_ = std::move(exception);
            }
            finished_ = true;
            std::cout << "mark finished" << std::endl;
        }
        for (const auto& sub_dep : sub_dependency_)
        {
            auto t = sub_dep.lock();
            if (t)
            {
                t->UpdateReadyStatus();
            }
        }
        for (const auto& sub_trg : sub_trigger_)
        {
            sub_trg->UpdateReadyStatus();
        }
        cv_.notify_all();
    }
    void Task::Cancel() { Finish(TaskStatus::Canceled); }
    void Task::SubmitTo(Executor* executor) { executor_.store(executor); }
    void Task::ResetExecutor() { executor_.store(nullptr); }
    void Task::BeginProcessing() { status_.store(TaskStatus::Processing); }
    void Task::UpdateReadyStatus()
    {
        auto executor = executor_.load();
        if (executor)
        {
            executor->NotifyAvailable();
        }
    }
    void Task::Wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        std::cout << this->finished_ << " flag" << std::endl;
        cv_.wait(lock, [this]() { return this->finished_; });
    }

    //*___WORKER_______________________________________________________________________________________________________________

    void TimerWorker(TimerQueue* timer)
    {
        std::unique_lock<std::mutex> lock(timer->mutex_);
        while (true)
        {
            if (!timer->working_)
            {
                return;
            }
            TimePoint t = std::chrono::system_clock::now() + std::chrono::seconds(1000000);
            if (!timer->tasks_.empty())
            {
                t = timer->tasks_.top()->execute_after_;
            }
            else
            {
            }
            auto status = timer->cv_.wait_until(lock, t);
            if (!timer->working_)
            {
                return;
            }
            if (status == std::cv_status::timeout)
            {
                timer->tasks_.top()->the_time_has_come_.store(true);
                timer->tasks_.top()->UpdateReadyStatus();
                timer->tasks_.pop();
                continue;
            }
            if (timer->update_)
            {
                timer->update_ = false;
                continue;
            }
        }
    }

    void Worker(size_t worker_id, Executor* executor)
    {
        (void)worker_id;
        while (true)
        {
            auto               task      = executor->WaitForTask();
            TaskStatus         status    = TaskStatus::Completed;
            std::exception_ptr exception = nullptr;
            if (!task)
            {
                break;
            }
            try
            {
                task->Run();
            } catch (const std::exception& e)
            {
                status    = TaskStatus::Failed;
                exception = std::current_exception();
            }
            std::cout << "task finished" << std::endl;
            task->Finish(status, exception);
        }
        executor->wait_for_task_.notify_one();
    }
}