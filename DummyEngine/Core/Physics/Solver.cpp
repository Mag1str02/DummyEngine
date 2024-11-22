#include "Solver.hpp"

#include "DummyEngine/Core/Physics/PhysicsComponent.h"
#include "DummyEngine/Core/Physics/Utils.hpp"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/Scene.h"

#include <algorithm>

namespace DummyEngine {

    void Physics::Solver::LoadScene(const Ref<Scene>& scene) {
        if (!scene) {
            return;
        }
        scene_             = scene;
        constants_.Gravity = Vec3(0, -4, 0);

        time_ = 0;
        for (auto entity : scene->View<RenderMeshComponent>()) {
            UUID id     = entity.Get<RenderMeshComponent>().ID;
            auto hitbox = ResourceManager::GetHitBox(id);
            if (hitbox) {
                colliders_.insert({entity.GetComponent<IDComponent>()->Get(), CreateRef<ConvexCollider>(*hitbox)});
            }
        }
    }

    void Physics::Solver::OnUpdate(double dt) {
        auto scene = scene_.lock();
        if (!scene) {
            return;
        }
        frame_++;
        double curr_dt = 0;
        while (curr_dt < dt) {
            curr_dt += NextInteraction(dt - curr_dt);
        }
    }

    double Physics::Solver::NextInteraction(double dt) {
        auto              scene    = scene_.lock();
        constexpr int     kSubtics = 10;
        std::vector<UUID> collidable_entities;
        for (auto entity : scene->View<PhysicsComponent>()) {
            auto& phys = entity.Get<PhysicsComponent>();
            if (phys.Collidable) {
                collidable_entities.push_back(entity.Get<IDComponent>().Get());
            }
        }
        float delta = dt / kSubtics;
        for (int subtick = 0; subtick < kSubtics; subtick++) {
            for (const auto& id : collidable_entities) {
                colliders_[id]->UpdateTransformationMatrix(scene->GetByID(id).Get<TransformComponent>().GetTransform());
            }
            for (auto entity : scene->View<PhysicsComponent>()) {
                auto& phys = entity.Get<PhysicsComponent>();
                if (phys.Gravity) {
                    phys.Speed += constants_.Gravity * delta;
                }
                //            phys.speed += phys.force * phys.inv_mass * delta;
                //            phys.rot_speed += phys.torque * phys.inv_inertia * delta;
                //            if (subtick == 0) {
                //                phys.speed *= 0.999f;
                //                phys.rot_speed *= 0.999f;
                //            }
            }
            for (const auto& lhs : collidable_entities) {
                for (const auto& rhs : collidable_entities) {
                    if (lhs == rhs) {
                        continue;
                    }
                    if (lhs.Hex() >= rhs.Hex()) continue;
                    auto lhs_collider     = colliders_[lhs];
                    auto rhs_collider     = colliders_[rhs];
                    auto collision_normal = lhs_collider->Collide(*rhs_collider);
                    if (collision_normal == Vec3(0)) {
                        auto it = Memory.find({lhs, rhs});
                        if (it != Memory.end()) {
                            Memory.erase(it);
                        }
                        continue;
                    }

                    bool warm_start = Memory.find({lhs, rhs}) != Memory.end();

                    // Calc collision point
                    int  lhs_cnt = 0, rhs_cnt = 0;
                    auto lhs_pt = lhs_collider->GetCollisionPoint(collision_normal, lhs_cnt);
                    auto rhs_pt = rhs_collider->GetCollisionPoint(-collision_normal, rhs_cnt);

                    // LOG_DEBUG("LHS_PT {}", lhs_pt);
                    // LOG_DEBUG("RHS_PT {}", rhs_pt);
                    float lhs_area = 0, rhs_area = 0;
                    auto  lhs_pts = rhs_collider->GetCollisionCenter(-collision_normal, lhs_pt, lhs_area);
                    auto  rhs_pts = lhs_collider->GetCollisionCenter(collision_normal, rhs_pt, rhs_area);

                    auto  collision_pts = lhs_pts;
                    Plane dst_pl(collision_normal, rhs_pt);
                    if (lhs_area < 0 || rhs_area < lhs_area) {
                        collision_pts = rhs_pts;
                        dst_pl        = Plane(-collision_normal, lhs_pt);
                    }
                    for (const auto& collision_pt : collision_pts) {
                        // LOG_DEBUG("CollisionPT {}", collision_pt);
                        float penetration = std::abs(dst_pl.Distance(collision_pt));

                        auto collision_normal_n = glm::normalize(collision_normal);
                        // LOG_DEBUG("CollisionNormal {}", collision_normal_n);
                        auto lhs_com = lhs_collider->TransformPoint(lhs_collider->GetCenterOfMass());
                        auto rhs_com = rhs_collider->TransformPoint(rhs_collider->GetCenterOfMass());

                        auto lhs_r = collision_pt - lhs_com;
                        auto rhs_r = collision_pt - rhs_com;
                        // LOG_DEBUG("LhsR {}", lhs_r);
                        // LOG_DEBUG("RhsR {}", rhs_r);

                        Vec3 tangent   = glm::normalize(glm::cross(collision_normal_n, glm::normalize(Vec3(1) + collision_normal_n)));
                        Vec3 tangent_b = glm::normalize(glm::cross(collision_normal_n, tangent));

                        auto list_iterator = Memory[{lhs, rhs}].begin();
                        if (warm_start) {
                            for (; list_iterator != Memory[{lhs, rhs}].end(); list_iterator++) {
                                Collision& cl = *list_iterator;
                                if (glm::length(collision_normal_n - cl.CollisionNormal) > 0.01) {
                                    continue;
                                }
                                if (glm::length(cl.LhsPt - collision_pt) < 0.001 && glm::length(cl.RhsPt - collision_pt) < 0.001) {
                                    break;
                                }
                            }
                        } else {
                            list_iterator = Memory[{lhs, rhs}].end();
                        }

                        if (!warm_start || list_iterator == Memory[{lhs, rhs}].end()) {
                            Collision col{
                                .Source          = lhs,
                                .Destination     = rhs,
                                .Origin          = collision_pt,
                                .CollisionNormal = collision_normal_n,
                                .Penetration     = penetration,
                                .LhsPt           = collision_pt,
                                .RhsPt           = collision_pt,
                                .LhsR            = lhs_r,
                                .RhsR            = rhs_r,
                            };
                            col.JN  = InitJacobian(col, collision_normal_n, delta, true);
                            col.JT  = InitJacobian(col, tangent, delta, false);
                            col.JTb = InitJacobian(col, tangent_b, delta, false);
                            Memory[{lhs, rhs}].push_back(col);
                            list_iterator = Memory[{lhs, rhs}].end();
                            list_iterator--;
                        }
                        Collision& col      = *list_iterator;
                        col.Origin          = collision_pt;
                        col.CollisionNormal = collision_normal_n;
                        col.Penetration     = penetration;
                        col.LhsPt           = collision_pt;
                        col.RhsPt           = collision_pt;
                        col.LhsR            = lhs_r;
                        col.RhsR            = rhs_r;
                        col.Frame           = frame_;
                    }
                }
            }

            for (auto& [ids, cols] : Memory) {
                for (S32 i = 0; i < S32(cols.size()); ++i) {
                    if (cols[i].Frame != frame_) {
                        cols.erase(cols.begin() + i);
                        --i;
                    }
                }
            }

            for (auto& [ids, cols] : Memory) {
                for (auto& col : cols) {
                    for (int i = 0; i < 25; i++) {
                        Resolve(col.JN, col, delta, true, nullptr);
                        Resolve(col.JT, col, delta, false, &col.JN);
                        Resolve(col.JTb, col, delta, false, &col.JN);
                    }
                }
            }

            for (auto entity : scene->View<PhysicsComponent>()) {
                auto& phys      = entity.Get<PhysicsComponent>();
                auto& transform = entity.Get<TransformComponent>();
                transform.Translation += phys.Speed * delta;
                transform.Rotation += phys.RotationSpeed * 360.f / static_cast<float>(2 * M_PI) * delta;
                transform.Rotation.x -= std::floor(transform.Rotation.x / 360) * 360;
                transform.Rotation.y -= std::floor(transform.Rotation.y / 360) * 360;
                transform.Rotation.z -= std::floor(transform.Rotation.z / 360) * 360;
            }
        }
        time_ += dt;
        return dt;
    }
    Physics::Jacobian Physics::Solver::InitJacobian(Physics::Collision& collision, Vec3 dir, float dt, bool is_normal) {
        auto     scene = scene_.lock();
        Jacobian j;
        j.VA = -dir;
        j.WA = -glm::cross(collision.LhsR, dir);
        j.VB = dir;
        j.WB = glm::cross(collision.RhsR, dir);

        j.Bias = .0f;

        auto lhs_phys = scene->GetByID(collision.Source).GetComponent<PhysicsComponent>();
        auto rhs_phys = scene->GetByID(collision.Destination).GetComponent<PhysicsComponent>();

        if (is_normal) {
            float beta = .5f;
            float rest = 0.01f;

            Vec3 relative_vel = -lhs_phys->Speed - glm::cross(lhs_phys->RotationSpeed, collision.LhsR) + rhs_phys->Speed +
                                glm::cross(rhs_phys->RotationSpeed, collision.RhsR);
            float closing_vel = glm::dot(relative_vel, dir);
            j.Bias            = -(beta / dt) * std::max(.0f, collision.Penetration - 0.01f) + rest * closing_vel;
        }

        float inv_mass = lhs_phys->InvertedMass + rhs_phys->InvertedMass + glm::dot(j.WA, lhs_phys->InvertedInertia * j.WA) +
                         glm::dot(j.WB, rhs_phys->InvertedInertia * j.WB);
        j.EffectiveMass = 1.f / inv_mass;
        j.TotatlLambda  = 0;
        return j;
    }
    void Physics::Solver::Resolve(Physics::Jacobian& j, Physics::Collision& collision, float, bool is_normal, Physics::Jacobian* jn) {
        auto scene = scene_.lock();

        auto lhs_phys = scene->GetByID(collision.Source).GetComponent<PhysicsComponent>();
        auto rhs_phys = scene->GetByID(collision.Destination).GetComponent<PhysicsComponent>();

        float jv = glm::dot(j.VA, lhs_phys->Speed) + glm::dot(j.WA, lhs_phys->RotationSpeed) + glm::dot(j.VB, rhs_phys->Speed) +
                   glm::dot(j.WB, rhs_phys->RotationSpeed);

        float lambda = j.EffectiveMass * (-(jv + j.Bias));

        float old_total_lambda = j.TotatlLambda;
        if (is_normal) {
            j.TotatlLambda = std::max(.0f, j.TotatlLambda + lambda);
        } else {
            float friction     = 0.2f;
            float max_friction = friction * jn->TotatlLambda;
            j.TotatlLambda     = std::max(-max_friction, std::min(max_friction, j.TotatlLambda + lambda));
        }
        lambda = j.TotatlLambda - old_total_lambda;

        lhs_phys->Speed += lhs_phys->InvertedMass * j.VA * lambda;
        lhs_phys->RotationSpeed += lhs_phys->InvertedInertia * j.WA * lambda;
        rhs_phys->Speed += rhs_phys->InvertedMass * j.VB * lambda;
        rhs_phys->RotationSpeed += rhs_phys->InvertedInertia * j.WB * lambda;
    }

}  // namespace DummyEngine
