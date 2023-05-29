#pragma once

#include <list>

#include "ConvexCollider.hpp"
#include "DummyEngine/Core/Scene/Scene.h"
#include "SceneConstants.hpp"

namespace DE {
    class Scene;
}

namespace DE::Physics {

    struct Jacobian {
        Vec3 m_va, m_wa, m_vb, m_wb;
        float m_bias, m_effective_mass, m_totalLambda;
    };

    struct Collision {
        UUID src, dest;
        Vec3 origin;
        Vec3 collision_normal;
        float penetration;

        Vec3 lhs_pt, rhs_pt;
        Vec3 lhs_r, rhs_r;

        Jacobian jN, jT, jTb;
    };


    class Solver {
    public:
        void LoadScene(const Ref<DE::Scene> &scene);

        void OnUpdate(double dt);
        double NextInteraction(double dt);
        Jacobian InitJacobian(Collision &collision, Vec3 dir, float dt, bool is_normal);
        void Resolve(Jacobian &j, Collision &collision, float dt, bool is_normal, Jacobian *jn);

        std::map<std::pair<UUID, UUID>, std::list<Collision>> mem;

    private:
        Ref<Scene> _scene;
        Ref<SceneConstants> _constants;
        std::unordered_map<UUID, Ref<ConvexCollider>> _colliders;
        double _time = 0;
    };

}