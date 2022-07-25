#pragma once

#include "Addition/Base.h"

namespace DE
{
    struct Transformation
    {
        Vec3 scale = Vec3(1.0), scale_offset = Vec3(0.0);
        Vec3 translation = Vec3(0.0), translation_offset = Vec3(0.0);
        Vec3 rotation = Vec3(0.0), rotation_offet = Vec3(0.0);

        Transformation() = default;

        Mat4 GetTransform() const;
    };
}  // namespace DE