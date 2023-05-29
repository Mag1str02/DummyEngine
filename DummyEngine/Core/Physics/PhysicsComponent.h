#pragma once

namespace DE::Physics {
    struct PhysicsComponent {
        Vec3 speed;
        Vec3 rot_speed;
        float inv_mass;
        float inv_inertia;
        bool collidable;
        bool gravity;

        Vec3 force;
        Vec3 torque;
    };
}