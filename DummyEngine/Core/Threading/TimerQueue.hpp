#pragma once

#include <DummyEngine/Utils/Base/STDIncludes.h>

#include "Task.hpp"

namespace DE {
    class TimerQueue {
    public:
        TimerQueue() = default;
        void AddTask(std::shared_ptr<Task> task);
        void Shutdown();

    private:
        friend void             TimerWorker(TimerQueue* timer);
        std::mutex              mutex_;
        std::condition_variable cv_;

        bool                                                                                working_ = true;
        bool                                                                                update_  = false;
        std::priority_queue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>, Cmp> tasks_;
    };
}  // namespace DE