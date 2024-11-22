#include "Utils.hpp"

namespace DummyEngine::Physics {

    Plane::Plane(Vec3 normal, double_t d) : normal_(normal), d_(d) {}
    Plane::Plane(Vec3 normal, Vec3 pt) : normal_(normal), d_(-glm::dot(normal, pt)) {}
    Vec3 Plane::IntersectionPoint(Vec3 p1, Vec3 p2) const {
        if (glm::dot(normal_, p2 - p1) == 0) {
            return (p1 + p2) / 2.f;
        }
        return p1 + (p2 - p1) * (-Distance(p1) / glm::dot(normal_, p2 - p1));
    };
    void Plane::Invert() {
        normal_ = -normal_;
    }
    Vec3 Plane::GetNormal() const {
        return normal_;
    }
    float Plane::Distance(Vec3 q) const {
        return glm::dot(normal_, q) + d_;
    }

}  // namespace DummyEngine::Physics