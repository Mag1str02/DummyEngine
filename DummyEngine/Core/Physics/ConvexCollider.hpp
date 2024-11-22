#pragma once

#include "DummyEngine/Utils/Debug/Logger.h"

#include <vector>

namespace DummyEngine::Physics {

    class ConvexHitbox {
    public:
        void Build(const std::vector<Vec3>& mesh);

    private:
        friend class ConvexCollider;
        Vec3                             center_of_mass_;
        std::vector<Vec3>                points_;
        std::vector<std::vector<size_t>> edges_;
        size_t                           size_;
    };

    class ConvexCollider {
        LOG_AUTHOR(ConvexCollider)
    public:
        ConvexCollider()                      = delete;
        ConvexCollider(const ConvexCollider&) = delete;

        explicit ConvexCollider(const Ref<ConvexHitbox>& hitbox);

        void              UpdateTransformationMatrix(const Mat4& transform);
        Vec3              GetPoint(size_t idx) const;
        Vec3              TransformPoint(const Vec3& vec) const;
        Vec3              Collide(const ConvexCollider& oth) const;
        Vec3              GetCollisionPoint(const Vec3& collision_normal, int& cnt) const;
        std::vector<Vec3> GetCollisionCenter(const Vec3& collision_normal, const Vec3& plane_pt, float& area) const;
        Vec3              Normalize(const Vec3& vec) const;

        Vec3 GetCenterOfMass() const;

    private:
        friend class ConvexHitbox;

        Ref<ConvexHitbox> hitbox_;
        Mat4              transform_{};
    };

    constexpr U32 kUniqueFaces = 4;
}  // namespace DummyEngine::Physics