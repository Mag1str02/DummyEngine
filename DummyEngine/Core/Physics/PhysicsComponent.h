#pragma once

namespace DE::Physics {
    struct PhysicsComponent {
        Vec3 center_of_mass;
        Vec3 speed;
        Vec3 rot_speed;
        double mass;
        bool collidable;
        bool gravity;
    };
}