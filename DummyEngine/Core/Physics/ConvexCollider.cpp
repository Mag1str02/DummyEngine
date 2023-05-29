#include "ConvexCollider.hpp"

#include "DummyEngine/Utils/Debug/Logger.h"
#include "DummyEngine/Core/Physics/Utils.cpp"

void DE::Physics::ConvexHitbox::Build(const std::vector<Vec3>& mesh) {
    float min_x = mesh[0].x, max_x = mesh[0].x;
    float min_y = mesh[0].y, max_y = mesh[0].y;
    float min_z = mesh[0].z, max_z = mesh[0].z;
    _center_of_mass = Vec3(0);
    _center_of_mass += mesh[0];
    for (size_t i = 1; i < mesh.size(); i++) {
        min_x = std::min(min_x, mesh[i].x);
        min_y = std::min(min_y, mesh[i].y);
        min_z = std::min(min_z, mesh[i].z);

        max_x = std::max(max_x, mesh[i].x);
        max_y = std::max(max_y, mesh[i].y);
        max_z = std::max(max_z, mesh[i].z);

        _center_of_mass += mesh[i];
    }

    _center_of_mass /= mesh.size();

    _points.emplace_back(min_x, min_y, min_z);
    _points.emplace_back(max_x, min_y, min_z);
    _points.emplace_back(min_x, max_y, min_z);
    _points.emplace_back(min_x, min_y, max_z);
    _points.emplace_back(max_x, max_y, min_z);
    _points.emplace_back(min_x, max_y, max_z);
    _points.emplace_back(max_x, min_y, max_z);
    _points.emplace_back(max_x, max_y, max_z);

    _edges.resize(8);

    _edges[0].push_back(1);
    _edges[0].push_back(2);
    _edges[0].push_back(3);

    _edges[1].push_back(0);
    _edges[1].push_back(4);
    _edges[1].push_back(6);

    _edges[2].push_back(0);
    _edges[2].push_back(4);
    _edges[2].push_back(5);

    _edges[3].push_back(0);
    _edges[3].push_back(5);
    _edges[3].push_back(6);

    _edges[4].push_back(1);
    _edges[4].push_back(2);
    _edges[4].push_back(7);

    _edges[5].push_back(2);
    _edges[5].push_back(3);
    _edges[5].push_back(7);

    _edges[6].push_back(1);
    _edges[6].push_back(3);
    _edges[6].push_back(7);

    _edges[7].push_back(4);
    _edges[7].push_back(5);
    _edges[7].push_back(6);

    _size = 8;
}

DE::Physics::ConvexCollider::ConvexCollider(const DE::Ref<DE::Physics::ConvexHitbox> &hitbox) : _hitbox(hitbox), _transform(1) {}

void DE::Physics::ConvexCollider::UpdateTransformationMatrix(const DE::Mat4& transform) {
    _transform = transform;
}

DE::Vec3 DE::Physics::ConvexCollider::GetPoint(const size_t idx) const {
    Vec4 cpt(_hitbox->_points[idx], 1);
    cpt = _transform * cpt;
    return cpt;
}

DE::Vec3 DE::Physics::ConvexCollider::Collide(const DE::Physics::ConvexCollider& oth) const {
    std::vector<DE::Vec3> axis;
    for(int i = 1; i < DE::Physics::UNIQUE_FACES; i++) {
        Vec3 A = GetPoint(i) - GetPoint(0);
        Vec3 B = GetPoint((i + 1) % (UNIQUE_FACES - 1) + 1) - GetPoint(0);
        axis.push_back(glm::normalize(glm::cross(A, B)));
    }

    for(int i = 1; i < DE::Physics::UNIQUE_FACES; i++) {
        Vec3 A = oth.GetPoint(i) - oth.GetPoint(0);
        Vec3 B = oth.GetPoint((i + 1) % (UNIQUE_FACES - 1) + 1) - oth.GetPoint(0);
        axis.push_back(glm::normalize(glm::cross(A, B)));
    }

    for(int i = 1; i < DE::Physics::UNIQUE_FACES; i++) {
        Vec3 A = GetPoint(i) - GetPoint(0);
        for(int j = 1; j < DE::Physics::UNIQUE_FACES; j++) {
            Vec3 B = oth.GetPoint((j + 1) % (UNIQUE_FACES - 1) + 1) - oth.GetPoint(0);
            Vec3 cross = glm::cross(A, B);
            if (glm::length(cross) == 0) {
                axis.push_back(glm::normalize(cross));
            }
        }
    }

    Vec3 norm(1e9);
    for(auto axle : axis) {
        float min_a, max_a, min_b, max_b;
        for(size_t i = 0; i < _hitbox->_points.size(); i++) {
            float proj = glm::dot(GetPoint(i), axle);
            if (i == 0 || proj < min_a) {
                min_a = proj;
            }
            if (i == 0 || proj > max_a) {
                max_a = proj;
            }
        }

        for(size_t i = 0; i < oth._hitbox->_points.size(); i++) {
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
            if(pts[0] != min_a) {
                norm *= -1;
            }
        }
    }
    return norm;
}
DE::Vec3 DE::Physics::ConvexCollider::GetCollisionPoint(const DE::Vec3& collisionNormal, int &cnt) const {
    Vec3 norm = glm::normalize(collisionNormal);
    float best = glm::dot(norm, GetPoint(0));
    for(const auto& pt: _hitbox->_points) {
        best = std::max(best, glm::dot(norm, TransformPoint(pt)));
    }
    Vec3 sum(0);
    cnt = 0;
    for(const auto& pt: _hitbox->_points) {
        auto tpt = TransformPoint(pt);
        if(std::abs(glm::dot(norm, TransformPoint(pt)) - best) < 1e-7) {
            cnt++;
            sum += TransformPoint(pt);
        }
    }
    return sum / static_cast<float>(cnt);
}
DE::Vec3 DE::Physics::ConvexCollider::Normalize(const DE::Vec3& vec) const {
    Vec4 cpt(vec, 1);
    cpt = glm::inverse(_transform) * cpt;
    return cpt;
}
DE::Vec3 DE::Physics::ConvexCollider::GetCenterOfMass() const {
    return _hitbox->_center_of_mass;
}
DE::Vec3 DE::Physics::ConvexCollider::TransformPoint(const DE::Vec3& vec) const {
    Vec4 cpt(vec, 1);
    cpt = _transform * cpt;
    return cpt;
}

std::vector<DE::Vec3> DE::Physics::ConvexCollider::GetCollisionCenter(const DE::Vec3& collisionNormal, const DE::Vec3& planePt, float& area) const {
    Plane pl(collisionNormal, planePt);
    std::vector<bool> side(_hitbox->_size, false);
    std::vector<Vec3> pts_res;
    for (size_t i = 0; i < _hitbox->_size; i++) {
        Vec3 pt = GetPoint(i);
        side[i] = (pl.distance(pt) >= 0);
    }
    for (size_t i = 0; i < _hitbox->_size; i++) {
        Vec3 lhs = GetPoint(i);
        if (pl.distance(lhs) == 0) {
            //LOG_DEBUG("GCC PTA", LOG_VEC(lhs));
            pts_res.push_back(lhs);
        }
        for (const auto &j: _hitbox->_edges[i]) {
            if (side[i] == side[j]) continue;
            Vec3 rhs = GetPoint(j);
            Vec3 pt = pl.intersectionPoint(lhs, rhs);
            //LOG_DEBUG("GCC PTI", LOG_VEC(pt));
            pts_res.push_back(pt);
        }
    }
    Vec3 center(0), U, V;
    bool calc_area = false;
    float x_min, y_min, x_max, y_max;
    if (pts_res.size() > 2) {
        calc_area = true;
        U = glm::normalize(pts_res[1] - pts_res[0]);
        V = glm::normalize(glm::cross(U, collisionNormal));


        float x = glm::dot(pts_res[0], U);
        float y = glm::dot(pts_res[0], V);
        x_min = x_max = x;
        y_min = y_max = y;
    } else {
        area = 0;
    }
    for(const Vec3 &pt: pts_res) {
        center += pt;
        if (calc_area) {
            float x = glm::dot(pt, U);
            float y = glm::dot(pt, V);
            x_min = std::min(x_min, x);
            x_max = std::max(x_max, x);
            y_min = std::min(y_min, y);
            y_max = std::max(y_max, y);
        }
    }
    if(pts_res.empty()) {
        area = -1;
    }
    if (calc_area) {
        area = (y_max - y_min) * (x_max - x_min); // we dont want real area, simple aabb's area should help a lot
    }
    return pts_res;
}
