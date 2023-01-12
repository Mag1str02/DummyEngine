#pragma once

#include <random>

namespace DE
{
    class Random
    {
    public:
        static float Float(float range_begin = 0.0f, float range_end = 1.0f);
        static int32_t Int(int32_t range_begin = 0, int32_t rang_end = 1024);
    };
}  // namespace DE