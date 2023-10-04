#pragma once

#include <DummyEngine/Utils/Base/STDIncludes.h>

#define TASK_ERROR_IF(expr, message) \
    if (expr) throw TaskError(message)

namespace DE {
    struct TaskError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct Cmp;
    class Executor;
    class TimerQueue;

    void TimerWorker(TimerQueue* timer);
    void Worker(size_t worker_id, Executor* executor);

    enum class TaskStatus { None = 0, Unassigned = 1, Submited = 2, Processing = 3, Canceled = 4, Failed = 5, Completed = 6 };
    using TimePoint = std::chrono::system_clock::time_point;

    class Task : public std::enable_shared_from_this<Task> {
    public:
        virtual ~Task() {}

        virtual void Run() = 0;

        void AddDependency(const std::shared_ptr<Task>& dep);
        void AddTrigger(const std::shared_ptr<Task>& dep);
        void SetTimeTrigger(TimePoint at);

        bool               IsReadyToExecute();
        bool               IsCompleted();
        bool               IsFailed();
        bool               IsCanceled();
        bool               IsFinished();
        bool               HasTimeTrigger() const;
        std::exception_ptr GetError();

        void Cancel();
        void Wait();

    private:
        friend void Worker(size_t worker_id, Executor* executor);
        friend void TimerWorker(TimerQueue* timer);
        friend class Executor;
        friend class TimerQueue;
        friend struct Cmp;

        void SubmitTo(Executor* executor);
        void ResetExecutor();
        void BeginProcessing();
        void Finish(TaskStatus status, std::exception_ptr ptr = nullptr);
        void UpdateReadyStatus();

        //*Result
        std::atomic<TaskStatus> status_   = {TaskStatus::Unassigned};
        std::atomic<Executor*>  executor_ = {nullptr};
        std::exception_ptr      e_ptr_    = nullptr;
        bool                    finished_ = false;

        //*Wait
        std::mutex              mutex_;
        std::condition_variable cv_;

        //*BeforeSubmit
        std::vector<std::weak_ptr<Task>>   sub_dependency_;
        std::vector<std::shared_ptr<Task>> sub_trigger_;
        TimePoint                          execute_after_;
        size_t                             dependency_count_ = 0;
        bool                               has_dependency_   = false;
        bool                               has_trigger_      = false;
        bool                               has_timer_        = false;
        bool                               no_condition_     = true;

        //*AfterSubmit
        std::atomic<size_t> finished_dependency_count_ = {0};
        std::atomic<bool>   triggered_                 = {false};
        std::atomic<bool>   the_time_has_come_         = {false};
    };

    // Used instead of void in generic code
    struct Unit {};

    enum class ExecutorState { None = 0, Working = 1, ShutDown = 2, Finished = 3 };

    struct Cmp {
        bool operator()(const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) const { return a->execute_after_ > b->execute_after_; }
    };
}  // namespace DE