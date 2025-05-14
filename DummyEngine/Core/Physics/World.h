#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "DummyEngine/Core/ECS/Entity.h"
#include "RigidBody.h"

namespace DummyEngine::Physics {

    class World {
    LOG_AUTHOR(World)
    public:
        World();
        World(const World& other)            = delete;
        World(World&& other)                 = delete;
        World& operator=(World&& other)      = delete;
        World& operator=(const World& other) = delete;
        ~World();

        void AddEntity(Entity entity);
        void Update(float delta_time);
        void SetGravity(const Vec3& gravity);

        reactphysics3d::PhysicsWorld* GetNativeWorld() { return world_; }
        const reactphysics3d::PhysicsWorld* GetNativeWorld() const { return world_; }
        reactphysics3d::PhysicsCommon& GetPhysicsCommon() { return *common_; }
        const reactphysics3d::PhysicsCommon& GetPhysicsCommon() const { return *common_; }

    private:
        reactphysics3d::PhysicsCommon* common_;
        reactphysics3d::PhysicsWorld* world_;
        std::vector<Entity> entities_;
    };

}