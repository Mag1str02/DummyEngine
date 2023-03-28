#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"

namespace DE {
    namespace Constants {
        const float DefaultCameraFOV       = 90.0f;
        const float DefaultCameraAspect    = 16.0f / 9.0f;
        const float DefaultCameraNearPlane = 0.1f;
        const float DefaultCameraFarPlane  = 2'000'000.0f;

        const uint32_t MaxMessageTypeLength = 10;
        const uint32_t MaxAuthorLength      = 20;
    }  // namespace Constants
}  // namespace DE