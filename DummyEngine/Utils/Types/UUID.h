#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"

namespace DE {
    class UUID {
    public:
        static UUID Generate();

        UUID()            = default;
        UUID(const UUID&) = default;
        UUID(const std::string& str);

        operator std::string() const;

        bool operator==(const UUID& other) const;
        bool operator!=(const UUID& other) const;
        bool operator<(const UUID& other) const;
        bool operator>(const UUID& other) const;

        std::string     Hex() const;
        static uint32_t HexSize() { return 32; }

    private:
        template <typename T> friend struct std::hash;
        uint64_t m_First  = 0;
        uint64_t m_Second = 0;
    };

}  // namespace DE

namespace std {
    template <typename T> struct hash;

    template <> struct hash<DE::UUID> {
        std::size_t operator()(const DE::UUID& uuid) const { return uuid.m_First ^ uuid.m_Second; }
    };

}  // namespace std