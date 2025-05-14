#pragma once

#include "Collider.h"
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <memory>

namespace DummyEngine::Physics {

    enum class BodyType {
        STATIC = 0,
        KINEMATIC = 1,
        DYNAMIC = 2
    };

    struct RigidBodyComponent {
        UUID ID;
        std::vector<Ref<Collider>> Colliders;
        bool IsGravity = true;
        BodyType BodyType = BodyType::STATIC;
        
        // Physics properties
        float Mass = 1.0f;
        float LinearDamping = 0.0f;
        float AngularDamping = 0.0f;
        bool AllowSleep = true;
        Vec3 LinearVelocity = Vec3(0.0f);
        Vec3 AngularVelocity = Vec3(0.0f);
        
        // Internal use
        reactphysics3d::RigidBody* NativeBody = nullptr;
    };

}