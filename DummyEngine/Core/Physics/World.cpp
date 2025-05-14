#include "World.h"
#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "glm/gtc/quaternion.hpp"

namespace DummyEngine::Physics {

World::World() {
    LOG_INFO("Creating physics world");
    common_ = new reactphysics3d::PhysicsCommon();
    world_ = common_->createPhysicsWorld();
    world_->setGravity(reactphysics3d::Vector3(0.0f, -9.81f, 0.0f));
}

World::~World() {
    // Cleanup all physics bodies
    for (auto entity : entities_) {
        if (entity.Has<RigidBodyComponent>()) {
            auto& rb = entity.Get<RigidBodyComponent>();
            if (rb.NativeBody) {
                world_->destroyRigidBody(rb.NativeBody);
                rb.NativeBody = nullptr;
            }
        }
    }
    common_->destroyPhysicsWorld(world_);
    delete common_;
}

void World::AddEntity(Entity entity) {
    LOG_INFO("Adding entity to physics world");
    if (!entity.Has<RigidBodyComponent>()) return;
    
    auto& rb = entity.Get<RigidBodyComponent>();
    auto& transform = entity.Get<TransformComponent>();
    
    // Create transform
    reactphysics3d::Transform physTransform;
    physTransform.setPosition(reactphysics3d::Vector3(
        transform.Translation.x,
        transform.Translation.y,
        transform.Translation.z
    ));
    auto rotation = glm::quat(glm::radians(transform.Rotation));
    physTransform.setOrientation(reactphysics3d::Quaternion(
        rotation.x,
        rotation.y,
        rotation.z,
        rotation.w
    ));
    
    // Create rigid body
    reactphysics3d::RigidBody* body = world_->createRigidBody(physTransform);
    
    // Set body type
    switch (rb.BodyType) {
        case BodyType::STATIC:
            body->setType(reactphysics3d::BodyType::STATIC);
            break;
        case BodyType::KINEMATIC:
            body->setType(reactphysics3d::BodyType::KINEMATIC);
            break;
        case BodyType::DYNAMIC:
            body->setType(reactphysics3d::BodyType::DYNAMIC);
            break;
    }
    
    // Set physics properties
    body->enableGravity(rb.IsGravity);
    body->setMass(rb.Mass);
    body->setLinearDamping(rb.LinearDamping);
    body->setAngularDamping(rb.AngularDamping);
    body->setIsAllowedToSleep(rb.AllowSleep);
    
    // Set velocities if any
    if (rb.LinearVelocity != Vec3(0.0f)) {
        body->setLinearVelocity(reactphysics3d::Vector3(
            rb.LinearVelocity.x,
            rb.LinearVelocity.y,
            rb.LinearVelocity.z
        ));
    }
    if (rb.AngularVelocity != Vec3(0.0f)) {
        body->setAngularVelocity(reactphysics3d::Vector3(
            rb.AngularVelocity.x,
            rb.AngularVelocity.y,
            rb.AngularVelocity.z
        ));
    }
    
    // Add colliders
    for (auto& collider : rb.Colliders) {
        collider->AttachToBody(*common_, body);
    }
    
    // Store the native body
    rb.NativeBody = body;
    entities_.push_back(entity);
}

void World::Update(float delta_time) {
    world_->update(delta_time);
    // Update entity transforms
    for (auto it = entities_.begin(); it != entities_.end();) {
        Entity entity = *it;
        // Skip invalid entities or those missing required components
        if (!entity.Valid() || !entity.Has<RigidBodyComponent>() || !entity.Has<TransformComponent>()) {
            it = entities_.erase(it);
            continue;
        }
        
        auto& rb = entity.Get<RigidBodyComponent>();
        if (rb.NativeBody) {
            auto& transform = entity.Get<TransformComponent>();
            const auto& physTransform = rb.NativeBody->getTransform();
            const auto& pos = physTransform.getPosition();
            const auto& rot = physTransform.getOrientation();
            
            transform.Translation = Vec3(pos.x, pos.y, pos.z);
            transform.Rotation = glm::degrees(glm::eulerAngles(glm::quat(rot.w, rot.x, rot.y, rot.z)));
        }
        
        ++it;
    }
}

void World::SetGravity(const Vec3& gravity) {
    world_->setGravity(reactphysics3d::Vector3(gravity.x, gravity.y, gravity.z));
}

}