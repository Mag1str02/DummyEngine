#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DE {
    class Random {
    public:
        static float Float(float range_begin = 0.0f, float range_end = 1.0f);
        static S32   Int(S32 range_begin = 0, S32 rang_end = 1024);
    };
}  // namespace DE