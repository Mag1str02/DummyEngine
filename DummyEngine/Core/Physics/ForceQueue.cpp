#include "ForceQueue.hpp"
namespace DE::Physics {

    bool operator<(const QueueObject& lhs, const QueueObject& rhs) {
        return lhs.dt < rhs.dt;
    }

    ForceQueue* ForceQueue::_instance = nullptr;

    ForceQueue* ForceQueue::getInstance() {
        if (ForceQueue::_instance == nullptr) {
            ForceQueue::_instance = new ForceQueue();
        }
        return ForceQueue::_instance;
    }

    void ForceQueue::ApplyForce(double dt, UUID entity_id, Vec3 dest, Vec3 force) {
        _queue.push(QueueObject{dt, entity_id, dest, force});
    }

    const QueueObject* ForceQueue::pull() {
        return &_queue.top();
    }

}  // namespace DE::Physics