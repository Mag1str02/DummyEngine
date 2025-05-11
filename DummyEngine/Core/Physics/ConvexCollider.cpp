#include "ConvexCollider.hpp"

#include "DummyEngine/Core/Physics/Utils.hpp"

#include <algorithm>

namespace DummyEngine {

    void Physics::ConvexHitbox::Build(const std::vector<Vec3>& mesh) {
        float min_x = mesh[0].x, max_x = mesh[0].x;
        float min_y = mesh[0].y, max_y = mesh[0].y;
        float min_z = mesh[0].z, max_z = mesh[0].z;
        center_of_mass_ = Vec3(0);
        center_of_mass_ += mesh[0];
        for (size_t i = 1; i < mesh.size(); i++) {
            min_x = std::min(min_x, mesh[i].x);
            min_y = std::min(min_y, mesh[i].y);
            min_z = std::min(min_z, mesh[i].z);

            max_x = std::max(max_x, mesh[i].x);
            max_y = std::max(max_y, mesh[i].y);
            max_z = std::max(max_z, mesh[i].z);

            center_of_mass_ += mesh[i];
        }

        center_of_mass_ /= mesh.size();

        points_.emplace_back(min_x, min_y, min_z);
        points_.emplace_back(max_x, min_y, min_z);
        points_.emplace_back(min_x, max_y, min_z);
        points_.emplace_back(min_x, min_y, max_z);
        points_.emplace_back(max_x, max_y, min_z);
        points_.emplace_back(min_x, max_y, max_z);
        points_.emplace_back(max_x, min_y, max_z);
        points_.emplace_back(max_x, max_y, max_z);

        edges_.resize(8);

        edges_[0].push_back(1);
        edges_[0].push_back(2);
        edges_[0].push_back(3);

        edges_[1].push_back(0);
        edges_[1].push_back(4);
        edges_[1].push_back(6);

        edges_[2].push_back(0);
        edges_[2].push_back(4);
        edges_[2].push_back(5);

        edges_[3].push_back(0);
        edges_[3].push_back(5);
        edges_[3].push_back(6);

        edges_[4].push_back(1);
        edges_[4].push_back(2);
        edges_[4].push_back(7);

        edges_[5].push_back(2);
        edges_[5].push_back(3);
        edges_[5].push_back(7);

        edges_[6].push_back(1);
        edges_[6].push_back(3);
        edges_[6].push_back(7);

        edges_[7].push_back(4);
        edges_[7].push_back(5);
        edges_[7].push_back(6);

        size_ = 8;
    }

    Physics::ConvexCollider::ConvexCollider(const Ref<Physics::ConvexHitbox>& hitbox) : hitbox_(hitbox), transform_(1) {}

    void Physics::ConvexCollider::UpdateTransformationMatrix(const Mat4& transform) {
        transform_ = transform;
    }

    Vec3 Physics::ConvexCollider::GetPoint(const size_t idx) const {
        Vec4 cpt(hitbox_->points_[idx], 1);
        cpt = transform_ * cpt;
        return cpt;
    }

    Vec3 Physics::ConvexCollider::Collide(const Physics::ConvexCollider& oth) const {
        std::vector<Vec3> axis;
        for (U32 i = 1; i < Physics::kUniqueFaces; ++i) {
            Vec3 a = GetPoint(i) - GetPoint(0);
            Vec3 b = GetPoint((i + 1) % (kUniqueFaces - 1) + 1) - GetPoint(0);
            axis.push_back(glm::normalize(glm::cross(a, b)));
        }

        for (U32 i = 1; i < Physics::kUniqueFaces; ++i) {
            Vec3 a = oth.GetPoint(i) - oth.GetPoint(0);
            Vec3 b = oth.GetPoint((i + 1) % (kUniqueFaces - 1) + 1) - oth.GetPoint(0);
            axis.push_back(glm::normalize(glm::cross(a, b)));
        }

        for (U32 i = 1; i < Physics::kUniqueFaces; ++i) {
            Vec3 a = GetPoint(i) - GetPoint(0);
            for (U32 j = 1; j < Physics::kUniqueFaces; ++j) {
                Vec3 b     = oth.GetPoint((j + 1) % (kUniqueFaces - 1) + 1) - oth.GetPoint(0);
                Vec3 cross = glm::cross(a, b);
                if (glm::length(cross) == 0) {
                    axis.push_back(glm::normalize(cross));
                }
            }
        }

        Vec3 norm(1e9);
        for (auto axle : axis) {
            float min_a = 0, max_a = 0, min_b = 0, max_b = 0;
            for (size_t i = 0; i < hitbox_->points_.size(); i++) {
                float proj = glm::dot(GetPoint(i), axle);
                if (i == 0 || proj < min_a) {
                    min_a = proj;
                }
                if (i == 0 || proj > max_a) {
                    max_a = proj;
                }
            }

            for (size_t i = 0; i < oth.hitbox_->points_.size(); i++) {
                float proj = glm::dot(oth.GetPoint(i), axle);
                if (i == 0 || proj < min_b) {
                    min_b = proj;
                }
                if (i == 0 || proj > max_b) {
                    max_b = proj;
                }
            }

            std::array<float, 4> pts{min_a, min_b, max_a, max_b};
            std::sort(pts.begin(), pts.end());

            float sum = max_a - min_a + max_b - min_b;
            float len = pts[3] - pts[0];
            if (sum <= len) {
                return Vec3(0);
            }
            float intersection_len = pts[2] - pts[1];
            if (intersection_len < glm::length(norm)) {
                norm = axle * intersection_len;
                if (pts[0] != min_a) {
                    norm *= -1;
                }
            }
        }
        return norm;
    }
    Vec3 Physics::ConvexCollider::GetCollisionPoint(const Vec3& collision_normal, int& cnt) const {
        Vec3  norm = glm::normalize(collision_normal);
        float best = glm::dot(norm, GetPoint(0));
        for (const auto& pt : hitbox_->points_) {
            best = std::max(best, glm::dot(norm, TransformPoint(pt)));
        }
        Vec3 sum(0);
        cnt = 0;
        for (const auto& pt : hitbox_->points_) {
            TransformPoint(pt);
            if (std::abs(glm::dot(norm, TransformPoint(pt)) - best) < 1e-7) {
                cnt++;
                sum += TransformPoint(pt);
            }
        }
        return sum / static_cast<float>(cnt);
    }
    Vec3 Physics::ConvexCollider::Normalize(const Vec3& vec) const {
        Vec4 cpt(vec, 1);
        cpt = glm::inverse(transform_) * cpt;
        return cpt;
    }
    Vec3 Physics::ConvexCollider::GetCenterOfMass() const {
        return hitbox_->center_of_mass_;
    }
    Vec3 Physics::ConvexCollider::TransformPoint(const Vec3& vec) const {
        Vec4 cpt(vec, 1);
        cpt = transform_ * cpt;
        return cpt;
    }

    std::vector<Vec3> Physics::ConvexCollider::GetCollisionCenter(const Vec3& collision_normal, const Vec3& plane_pt, float& area) const {
        Plane             pl(collision_normal, plane_pt);
        std::vector<bool> side(hitbox_->size_, false);
        std::vector<Vec3> pts_res;
        for (size_t i = 0; i < hitbox_->size_; i++) {
            Vec3 pt = GetPoint(i);
            side[i] = (pl.Distance(pt) >= 0);
        }
        for (size_t i = 0; i < hitbox_->size_; i++) {
            Vec3 lhs = GetPoint(i);
            if (pl.Distance(lhs) == 0) {
                // LOG_DEBUG("GCC PTA {}", lhs);
                pts_res.push_back(lhs);
            }
            for (const auto& j : hitbox_->edges_[i]) {
                if (side[i] == side[j]) continue;
                Vec3 rhs = GetPoint(j);
                Vec3 pt  = pl.IntersectionPoint(lhs, rhs);
                // LOG_DEBUG("GCC PTI {}", pt);
                pts_res.push_back(pt);
            }
        }
        Vec3  center(0), u, v;
        bool  calc_area = false;
        float x_min, y_min, x_max, y_max;
        if (pts_res.size() > 2) {
            calc_area = true;
            u         = glm::normalize(pts_res[1] - pts_res[0]);
            v         = glm::normalize(glm::cross(u, collision_normal));

            float x = glm::dot(pts_res[0], u);
            float y = glm::dot(pts_res[0], v);
            x_min = x_max = x;
            y_min = y_max = y;
        } else {
            area = 0;
        }
        for (const Vec3& pt : pts_res) {
            center += pt;
            if (calc_area) {
                float x = glm::dot(pt, u);
                float y = glm::dot(pt, v);
                x_min   = std::min(x_min, x);
                x_max   = std::max(x_max, x);
                y_min   = std::min(y_min, y);
                y_max   = std::max(y_max, y);
            }
        }
        if (pts_res.empty()) {
            area = -1;
        }
        if (calc_area) {
            area = (y_max - y_min) * (x_max - x_min);  // we dont want real area, simple aabb's area should help a lot
        }
        return pts_res;
    }

}  // namespace DummyEngine
