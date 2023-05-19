#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Types/Types.h"

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

        std::string Hex() const;
        static U32  HexSize() { return 32; }

    private:
        template <typename T> friend struct std::hash;
        U64 m_First  = 0;
        U64 m_Second = 0;
    };

}  // namespace DE

namespace std {
    template <> struct hash<DE::UUID> {
        std::size_t operator()(const DE::UUID& uuid) const { return uuid.m_First ^ uuid.m_Second; }
    };

}  // namespace std