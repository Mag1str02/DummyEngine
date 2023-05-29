#pragma once

#include "DummyEngine/Utils/Types/Types.h"
#include "DummyEngine/Utils/Types/UUID.h"

namespace DE::Physics {

    struct QueueObject {
        double dt;

        UUID entity_id;

        Vec3 dest, force;
    };

    class ForceQueue {
        static ForceQueue* getInstance();
        const QueueObject* pull();
        void               ApplyForce(double dt, UUID entity_id, Vec3 dest, Vec3 force);

    private:
        std::priority_queue<QueueObject> _queue;
        static ForceQueue*               _instance;
    };

}  // namespace DE::Physics