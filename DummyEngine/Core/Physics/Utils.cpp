#include "DummyEngine/Utils/Types/Types.h"
namespace DE::Physics {

    class Plane {
    public:
        Plane(Vec3 normal, double_t d) : _normal(normal), _d(d) {}
        Plane(Vec3 normal, Vec3 pt) : _normal(normal), _d(-glm::dot(normal, pt)) {}
        Plane(Vec3 v1, Vec3 v2, Vec3 v3) : _v1(v1), _v2(v2), _v3(v3) {
            _normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            _d      = -glm::dot(_normal, v1);
        }
        Vec3    intersectionPoint(Vec3 p1, Vec3 p2) const { return p1 + (p2 - p1) * (-distance(p1) / glm::dot(_normal, p2 - p1)); };
        void    invert() { _normal = -_normal; }
        Vec3    getNormal() const { return _normal; }
        float distance(Vec3 q) const { return glm::dot(_normal, q) + _d; }

    private:
        Vec3     _normal, _v1, _v2, _v3;
        float _d;
    };

}