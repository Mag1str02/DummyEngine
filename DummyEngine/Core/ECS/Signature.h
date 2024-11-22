#pragma once

#include "Forward.h"

namespace DummyEngine {

    class Signature {
    public:
        bool   Get(U64 id) const;
        void   Set(U64 id, bool value);
        bool   Matches(const Signature& required) const;
        size_t Size() const;

    private:
        std::vector<U64> data_;
    };

}  // namespace DummyEngine