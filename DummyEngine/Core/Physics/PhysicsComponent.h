#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine::Physics {

    struct PhysicsComponent {
        Vec3  Speed;
        Vec3  RotationSpeed;
        float InvertedMass;
        float InvertedInertia;
        bool  Collidable;
        bool  Gravity;

        Vec3 Force;
        Vec3 Torque;
    };

}  // namespace DummyEngine::Physics