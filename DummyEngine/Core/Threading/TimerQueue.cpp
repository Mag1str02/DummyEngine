#include "TimerQueue.hpp"

namespace DE {
    void TimerQueue::AddTask(std::shared_ptr<Task> task) {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(task);
        update_ = true;
        cv_.notify_one();
    }
    void TimerQueue::Shutdown() {
        std::unique_lock<std::mutex> lock(mutex_);
        working_ = false;
        lock.unlock();
        cv_.notify_one();
    }
}  // namespace DE