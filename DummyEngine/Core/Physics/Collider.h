#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "reactphysics3d/body/RigidBody.h"
#include "reactphysics3d/collision/Collider.h"
#include "reactphysics3d/engine/PhysicsCommon.h"
#include "reactphysics3d/mathematics/Transform.h"
#include "reactphysics3d/mathematics/Vector3.h"

namespace DummyEngine::Physics {

    class Collider {
    public:
        virtual ~Collider() = default;
        virtual reactphysics3d::Collider* AttachToBody(reactphysics3d::PhysicsCommon &common, reactphysics3d::RigidBody* body) = 0;
    };

    class CubeCollider : public Collider {
    public:
        CubeCollider() = delete;
        CubeCollider(Vec3 size, Vec3 local_pos) : size_(size), local_pos_(local_pos) {}
        
        virtual reactphysics3d::Collider* AttachToBody(reactphysics3d::PhysicsCommon &common, reactphysics3d::RigidBody* body) override {
            auto shape = common.createBoxShape(reactphysics3d::Vector3(size_.x, size_.y, size_.z));
            reactphysics3d::Transform transform = reactphysics3d::Transform::identity();
            transform.setPosition(reactphysics3d::Vector3(local_pos_.x, local_pos_.y, local_pos_.z));
            return body->addCollider(shape, transform);
        }

        const Vec3& GetSize() const { return size_; }
        void SetSize(const Vec3& size) { size_ = size; }

        const Vec3& GetLocalPosition() const { return local_pos_; }
        void SetLocalPosition(const Vec3& pos) { local_pos_ = pos; }

    private:
        Vec3 size_;
        Vec3 local_pos_;
    };

}