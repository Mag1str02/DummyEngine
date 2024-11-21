#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine::Physics {

    class Plane {
    public:
        Plane(Vec3 normal, double_t d);
        Plane(Vec3 normal, Vec3 pt);

        Vec3  IntersectionPoint(Vec3 p1, Vec3 p2) const;
        void  Invert();
        Vec3  GetNormal() const;
        float Distance(Vec3 q) const;

    private:
        Vec3  normal_;
        float d_;
    };

}  // namespace DummyEngine::Physics