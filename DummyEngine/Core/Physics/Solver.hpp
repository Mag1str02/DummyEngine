#pragma once

#include "DummyEngine/Core/Physics/ConvexCollider.hpp"

#include <map>
#include <vector>

namespace DummyEngine {
    class Scene;
}

namespace DummyEngine::Physics {

    struct Jacobian {
        Vec3  VA, WA, VB, WB;
        float Bias;
        float EffectiveMass;
        float TotatlLambda;
    };

    struct Collision {
        UUID  Source, Destination;
        Vec3  Origin;
        Vec3  CollisionNormal;
        float Penetration;

        Vec3 LhsPt, RhsPt;
        Vec3 LhsR, RhsR;

        Jacobian JN, JT, JTb;
        size_t   Frame;
    };

    struct SceneConstants {
        Vec3 Gravity;
    };

    class Solver {
    public:
        void LoadScene(const Ref<DummyEngine::Scene>& scene);

        void     OnUpdate(double dt);
        double   NextInteraction(double dt);
        Jacobian InitJacobian(Collision& collision, Vec3 dir, float dt, bool is_normal);
        void     Resolve(Jacobian& j, Collision& collision, float dt, bool is_normal, Jacobian* jn);

        std::map<std::pair<UUID, UUID>, std::vector<Collision>> Memory;

    private:
        WeakRef<Scene>                                scene_;
        SceneConstants                                constants_;
        std::unordered_map<UUID, Ref<ConvexCollider>> colliders_;
        double                                        time_  = 0;
        size_t                                        frame_ = 0;
    };

}  // namespace DummyEngine::Physics