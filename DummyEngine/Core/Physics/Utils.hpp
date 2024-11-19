#include "DummyEngine/Utils/Types/Types.h"

namespace DE::Physics {

    class Plane {
    public:
        Plane(Vec3 normal, double_t d);
        Plane(Vec3 normal, Vec3 pt);

        Vec3  intersectionPoint(Vec3 p1, Vec3 p2) const;
        void  invert();
        Vec3  getNormal() const;
        float distance(Vec3 q) const;

    private:
        Vec3  m_Normal;
        float m_D;
    };

}  // namespace DE::Physics