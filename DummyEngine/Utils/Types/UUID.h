#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <string>

namespace DummyEngine {
    class UUID {
    public:
        static UUID Generate();

        UUID() = default;
        UUID(const std::string& str);  // NOLINT
        operator std::string() const;  // NOLINT

        bool operator==(const UUID& other) const;
        bool operator!=(const UUID& other) const;
        bool operator<(const UUID& other) const;
        bool operator>(const UUID& other) const;

        std::string Hex() const;
        static U32  HexSize() { return 32; }

    private:
        template <typename T> friend struct std::hash;
        U64 first_  = 0;
        U64 second_ = 0;
    };

}  // namespace DummyEngine

namespace std {
    template <> struct hash<DummyEngine::UUID> {
        std::size_t operator()(const DummyEngine::UUID& uuid) const { return uuid.first_ ^ uuid.second_; }
    };

}  // namespace std