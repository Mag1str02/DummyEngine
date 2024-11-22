#include "Random.h"

#include <random>

namespace DummyEngine {

    static thread_local std::mt19937 gGenerator(time(nullptr));

    float Random::Float(float range_begin, float range_end) {
        float tmp = ((float)gGenerator() / (float)std::numeric_limits<U32>::max());
        return tmp * range_end + (1 - tmp) * range_begin;
    }
    S32 Random::Int(S32 range_begin, S32 range_end) {
        return Float(range_begin, range_end);
    }

}  // namespace DummyEngine