#include "Solver.hpp"

#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "PhysicsComponent.h"
#include "Utils.cpp"
void DE::Physics::Solver::LoadScene(const DE::Ref<DE::Scene>& scene) {
    if (!scene) {
        return;
    }
    _scene     = scene;
    _constants = CreateRef<SceneConstants>();
    _constants->setGravity(Vec3(0, -4, 0));
    _time = 0;
    for (auto entity : scene->View<RenderMeshComponent>()) {
        UUID id     = entity.Get<RenderMeshComponent>().id;
        auto hitbox = ResourceManager::GetHitBox(id);
        if (hitbox) {
            _colliders.insert({entity.GetComponent<IDComponent>()->Get(), CreateRef<ConvexCollider>(*hitbox)});
        }
    }
}

void DE::Physics::Solver::OnUpdate(double dt) {
    auto scene = _scene.lock();
    if (!scene) {
        return;
    }
    _frame++;
    double curr_dt = 0;
    while (curr_dt < dt) {
        curr_dt += NextInteraction(dt - curr_dt);
    }
}

double DE::Physics::Solver::NextInteraction(double dt) {
    auto              scene    = _scene.lock();
    const int         SUBTICKS = 10;
    std::vector<UUID> collidableEntities;
    for (auto entity : scene->View<PhysicsComponent>()) {
        auto& phys = entity.Get<PhysicsComponent>();
        if (phys.collidable) {
            collidableEntities.push_back(entity.Get<IDComponent>().Get());
        }
    }
    float delta = dt / SUBTICKS;
    for (int subtick = 0; subtick < SUBTICKS; subtick++) {
        for (const auto& id : collidableEntities) {
            _colliders[id]->UpdateTransformationMatrix(scene->GetByID(id).Get<TransformComponent>().GetTransform());
        }
        for (auto entity : scene->View<PhysicsComponent>()) {
            auto& phys = entity.Get<PhysicsComponent>();
            if (phys.gravity) {
                phys.speed += _constants->getGravity() * delta;
            }
            //            phys.speed += phys.force * phys.inv_mass * delta;
            //            phys.rot_speed += phys.torque * phys.inv_inertia * delta;
            //            if (subtick == 0) {
            //                phys.speed *= 0.999f;
            //                phys.rot_speed *= 0.999f;
            //            }
        }
        for (const auto& lhs : collidableEntities) {
            for (const auto& rhs : collidableEntities) {
                if (lhs == rhs) {
                    continue;
                }
                if (lhs.Hex() >= rhs.Hex()) continue;
                auto lhsCollider     = _colliders[lhs];
                auto rhsCollider     = _colliders[rhs];
                auto collisionNormal = lhsCollider->Collide(*rhsCollider);
                if (collisionNormal == Vec3(0)) {
                    if (mem.find({lhs, rhs}) != mem.end()) {
                        mem.erase(mem.find({lhs, rhs}));
                    }
                    continue;
                }

                bool warmStart = mem.find({lhs, rhs}) != mem.end();

                // Calc collision point
                int  lhs_cnt = 0, rhs_cnt = 0;
                auto lhs_pt = lhsCollider->GetCollisionPoint(collisionNormal, lhs_cnt);
                auto rhs_pt = rhsCollider->GetCollisionPoint(-collisionNormal, rhs_cnt);

                LOG_DEBUG("LHS_PT", LOG_VEC(lhs_pt));
                LOG_DEBUG("RHS_PT", LOG_VEC(rhs_pt));
                float lhs_area = 0, rhs_area = 0;
                auto  rhs_pts = lhsCollider->GetCollisionCenter(collisionNormal, rhs_pt, rhs_area);
                auto  lhs_pts = rhsCollider->GetCollisionCenter(-collisionNormal, lhs_pt, lhs_area);

                auto  collision_pts = lhs_pts;
                Plane dst_pl(collisionNormal, rhs_pt);
                if (lhs_area < 0 || rhs_area < lhs_area) {
                    collision_pts = rhs_pts;
                    dst_pl        = Plane(-collisionNormal, lhs_pt);
                }
                for (const auto& collision_pt : collision_pts) {
                    LOG_DEBUG("CollisionPT", LOG_VEC(collision_pt));
                    float penetration = std::abs(dst_pl.distance(collision_pt));

                    auto collisionNormal_n = glm::normalize(collisionNormal);
                    LOG_DEBUG("CollisionNormal", LOG_VEC(collisionNormal_n));
                    auto lhs_com = lhsCollider->TransformPoint(lhsCollider->GetCenterOfMass());
                    auto rhs_com = rhsCollider->TransformPoint(rhsCollider->GetCenterOfMass());

                    auto lhs_r = collision_pt - lhs_com;
                    auto rhs_r = collision_pt - rhs_com;
                    LOG_DEBUG("lhs_r", LOG_VEC(lhs_r));
                    LOG_DEBUG("rhs_r", LOG_VEC(rhs_r));

                    Vec3 tangent   = glm::normalize(glm::cross(collisionNormal_n, glm::normalize(Vec3(1) + collisionNormal_n)));
                    Vec3 tangent_b = glm::normalize(glm::cross(collisionNormal_n, tangent));

                    auto list_iterator = mem[{lhs, rhs}].begin();
                    if (warmStart) {
                        for (; list_iterator != mem[{lhs, rhs}].end(); list_iterator++) {
                            Collision& cl = *list_iterator;
                            if (glm::length(collisionNormal_n - cl.collision_normal) > 0.01) {
                                continue;
                            }
                            if (glm::length(cl.lhs_pt - collision_pt) < 0.001 && glm::length(cl.rhs_pt - collision_pt) < 0.001) {
                                break;
                            }
                        }
                    } else {
                        list_iterator = mem[{lhs, rhs}].end();
                    }

                    if (!warmStart || list_iterator == mem[{lhs, rhs}].end()) {
                        Collision col{lhs, rhs, collision_pt, collisionNormal_n, penetration, collision_pt, collision_pt, lhs_r, rhs_r};
                        col.jN  = InitJacobian(col, collisionNormal_n, delta, true);
                        col.jT  = InitJacobian(col, tangent, delta, false);
                        col.jTb = InitJacobian(col, tangent_b, delta, false);
                        mem[{lhs, rhs}].push_back(col);
                        list_iterator = mem[{lhs, rhs}].end();
                        list_iterator--;
                    }
                    Collision& col       = *list_iterator;
                    col.origin           = collision_pt;
                    col.collision_normal = collisionNormal_n;
                    col.penetration      = penetration;
                    col.lhs_pt           = collision_pt;
                    col.rhs_pt           = collision_pt;
                    col.lhs_r            = lhs_r;
                    col.rhs_r            = rhs_r;
                    col.frame            = _frame;
                }
            }
        }

        for (auto& [ids, cols] : mem) {
            cols.remove_if([&](const auto& el) { return el.frame != _frame; });
        }

        for (auto& [ids, cols] : mem) {
            for (auto& col : cols) {
                for (int i = 0; i < 25; i++) {
                    Resolve(col.jN, col, delta, true, nullptr);
                    Resolve(col.jT, col, delta, false, &col.jN);
                    Resolve(col.jTb, col, delta, false, &col.jN);
                    //                    LOG_DEBUG("COLLISION", "================");
                    //                    LOG_DEBUG("Origin", LOG_VEC(col.origin));
                    //                    LOG_DEBUG("collision_normal", LOG_VEC(col.collision_normal));
                    //                    LOG_DEBUG("collision_pt", LOG_VEC(col.lhs_pt));
                    //                    LOG_DEBUG("lhs_r", LOG_VEC(col.lhs_r));
                    //                    LOG_DEBUG("rhs_r", LOG_VEC(col.rhs_r));
                    //                    LOG_DEBUG("penetration", std::to_string(col.penetration));
                    //                    LOG_DEBUG("jnL", std::to_string(col.jN.m_totalLambda));
                    //                    LOG_DEBUG("jtL", std::to_string(col.jT.m_totalLambda));
                    //                    LOG_DEBUG("jt2L", std::to_string(col.jTb.m_totalLambda));
                    //                    LOG_DEBUG("COLLISION", "================");
                }
            }
        }

        for (auto entity : scene->View<PhysicsComponent>()) {
            auto& phys      = entity.Get<PhysicsComponent>();
            auto& transform = entity.Get<TransformComponent>();
            transform.translation += phys.speed * delta;
            transform.rotation += phys.rot_speed * 360.f / static_cast<float>(2 * M_PI) * delta;
            transform.rotation.x -= std::floor(transform.rotation.x / 360) * 360;
            transform.rotation.y -= std::floor(transform.rotation.y / 360) * 360;
            transform.rotation.z -= std::floor(transform.rotation.z / 360) * 360;
        }
    }
    _time += dt;
    return dt;
}
DE::Physics::Jacobian DE::Physics::Solver::InitJacobian(DE::Physics::Collision& collision, DE::Vec3 dir, float dt, bool is_normal) {
    auto     scene = _scene.lock();
    Jacobian j;
    j.m_va = -dir;
    j.m_wa = -glm::cross(collision.lhs_r, dir);
    j.m_vb = dir;
    j.m_wb = glm::cross(collision.rhs_r, dir);

    j.m_bias = .0f;

    auto lhs_phys = scene->GetByID(collision.src).GetComponent<PhysicsComponent>();
    auto rhs_phys = scene->GetByID(collision.dest).GetComponent<PhysicsComponent>();

    if (is_normal) {
        float beta = .5f;
        float rest = 0.01f;

        Vec3 relativeVel =
            -lhs_phys->speed - glm::cross(lhs_phys->rot_speed, collision.lhs_r) + rhs_phys->speed + glm::cross(rhs_phys->rot_speed, collision.rhs_r);
        float closingVel = glm::dot(relativeVel, dir);
        j.m_bias         = -(beta / dt) * std::max(.0f, collision.penetration - 0.01f) + rest * closingVel;
    }

    float inv_mass =
        lhs_phys->inv_mass + rhs_phys->inv_mass + glm::dot(j.m_wa, lhs_phys->inv_inertia * j.m_wa) + glm::dot(j.m_wb, rhs_phys->inv_inertia * j.m_wb);
    j.m_effective_mass = 1.f / inv_mass;
    j.m_totalLambda    = 0;
    return j;
}
void DE::Physics::Solver::Resolve(DE::Physics::Jacobian& j, DE::Physics::Collision& collision, float dt, bool is_normal, DE::Physics::Jacobian* jn) {
    auto scene = _scene.lock();
    Vec3 dir   = j.m_vb;

    auto lhs_phys = scene->GetByID(collision.src).GetComponent<PhysicsComponent>();
    auto rhs_phys = scene->GetByID(collision.dest).GetComponent<PhysicsComponent>();

    float jv = glm::dot(j.m_va, lhs_phys->speed) + glm::dot(j.m_wa, lhs_phys->rot_speed) + glm::dot(j.m_vb, rhs_phys->speed) +
               glm::dot(j.m_wb, rhs_phys->rot_speed);

    float lambda = j.m_effective_mass * (-(jv + j.m_bias));

    float oldTotalLambda = j.m_totalLambda;
    if (is_normal) {
        j.m_totalLambda = std::max(.0f, j.m_totalLambda + lambda);
    } else {
        float friction    = 0.2f;
        float maxFriction = friction * jn->m_totalLambda;
        j.m_totalLambda   = std::max(-maxFriction, std::min(maxFriction, j.m_totalLambda + lambda));
    }
    lambda = j.m_totalLambda - oldTotalLambda;

    lhs_phys->speed += lhs_phys->inv_mass * j.m_va * lambda;
    lhs_phys->rot_speed += lhs_phys->inv_inertia * j.m_wa * lambda;
    rhs_phys->speed += rhs_phys->inv_mass * j.m_vb * lambda;
    rhs_phys->rot_speed += rhs_phys->inv_inertia * j.m_wb * lambda;
}
