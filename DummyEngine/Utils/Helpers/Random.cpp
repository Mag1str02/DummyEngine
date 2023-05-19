#include "DummyEngine/Utils/Helpers/Random.h"

#include "DummyEngine/Utils/Types/Types.h"

namespace DE {

    std::mt19937 s_Generator(time(0));

    float Random::Float(float range_begin, float range_end) {
        float tmp = ((float)s_Generator() / (float)std::numeric_limits<U32>::max());
        return tmp * range_end + (1 - tmp) * range_begin;
    }
    S32 Random::Int(S32 range_begin, S32 range_end) {
        return Float(range_begin, range_end);
    }
}  // namespace DE