#include "DummyEngine/Core/ECS/Signature.h"

namespace DummyEngine {

#ifdef ECS_IMPLEMENTATION
    size_t Signature::Size() const {
        size_t size = 0;
        for (size_t i = 0; i < 64 * data_.size(); ++i) {
            if (Get(i)) {
                ++size;
            }
        }
        return size;
    }
    bool Signature::Get(U64 id) const {
        if (data_.size() <= (id >> 6)) {
            return false;
        }
        return ((data_[id >> 6] >> (id & 63)) & 1) != 0;
    }
    void Signature::Set(U64 id, bool value) {
        if (data_.size() <= id >> 6) {
            data_.resize((id >> 6) + 1);
        }
        data_[id >> 6] &= ~(1 << (id & 63));
        data_[id >> 6] |= ((U64)value << (id & 63));
    }
    bool Signature::Matches(const Signature& required) const {
        size_t i;
        size_t mn = std::min(required.data_.size(), data_.size());
        for (i = 0; i < mn; ++i) {
            if ((data_[i] & required.data_[i]) != required.data_[i]) {
                return false;
            }
        }
        while (i < required.data_.size()) {
            if (required.data_[i++] != 0) {
                return false;
            }
        }
        return true;
    }

#endif

}  // namespace DummyEngine
