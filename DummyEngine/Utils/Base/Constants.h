#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DE {
    namespace Constants {
        const float DefaultCameraFOV       = 90.0f;
        const float DefaultCameraAspect    = 16.0f / 9.0f;
        const float DefaultCameraNearPlane = 0.1f;
        const float DefaultCameraFarPlane  = 2'000'000.0f;

        const U32 MaxMessageTypeLength = 10;
        const U32 MaxAuthorLength      = 20;
    }  // namespace Constants
}  // namespace DE