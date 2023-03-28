#include "DummyEngine/Utils/Helpers/Random.h"

namespace DE
{

    std::mt19937 s_Generator(time(0));

    float Random::Float(float range_begin, float range_end)
    {
        float tmp = ((float)s_Generator() / (float)std::numeric_limits<uint32_t>::max());
        return tmp * range_end + (1 - tmp) * range_begin;
    }
    int32_t Random::Int(int32_t range_begin, int32_t range_end) { return Float(range_begin, range_end); }
}  // namespace DE