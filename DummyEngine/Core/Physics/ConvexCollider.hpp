#pragma once

#include <vector>

#include "DummyEngine/Utils/Types/Types.h"
#define LOG_VEC(p) std::to_string(p.x), " ", std::to_string(p.y), " ", std::to_string(p.z), " "
namespace DE::Physics {

    class ConvexHitbox {
    public:
        void Build(const std::vector<Vec3> &mesh);
    private:
        friend class ConvexCollider;
        Vec3 _center_of_mass;
        std::vector<Vec3> _points;
        std::vector<std::vector<size_t>> _edges;
        size_t _size;
    };

    class ConvexCollider {
    public:
        ConvexCollider() = delete;
        ConvexCollider(const ConvexCollider&) = delete;

        explicit ConvexCollider(const Ref<ConvexHitbox> &hitbox);

        void UpdateTransformationMatrix(const Mat4 &transform);
        Vec3 GetPoint(size_t idx) const;
        Vec3 TransformPoint(const Vec3 &vec) const;
        Vec3 Collide(const ConvexCollider &oth) const;
        Vec3 GetCollisionPoint(const Vec3 &collisionNormal, int &cnt) const;
        Vec3 GetCollisionCenter(const Vec3 &collisionNormal, const Vec3 &planePt, float &area) const;
        Vec3 Normalize(const Vec3 &vec) const;

        Vec3 GetCenterOfMass() const;
    private:
        friend class ConvexHitbox;

        Ref<ConvexHitbox> _hitbox;
        Mat4 _transform{};
    };

    const size_t UNIQUE_FACES = 4;
}  // namespace DE::Physics