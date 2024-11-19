#include "Utils.hpp"

namespace DE::Physics {

    Plane::Plane(Vec3 normal, double_t d) : m_Normal(normal), m_D(d) {}
    Plane::Plane(Vec3 normal, Vec3 pt) : m_Normal(normal), m_D(-glm::dot(normal, pt)) {}
    Vec3 Plane::intersectionPoint(Vec3 p1, Vec3 p2) const {
        if (glm::dot(m_Normal, p2 - p1) == 0) {
            return (p1 + p2) / 2.f;
        }
        return p1 + (p2 - p1) * (-distance(p1) / glm::dot(m_Normal, p2 - p1));
    };
    void Plane::invert() {
        m_Normal = -m_Normal;
    }
    Vec3 Plane::getNormal() const {
        return m_Normal;
    }
    float Plane::distance(Vec3 q) const {
        return glm::dot(m_Normal, q) + m_D;
    }

}  // namespace DE::Physics