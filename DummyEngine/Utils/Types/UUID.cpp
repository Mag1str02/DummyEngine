#include "DummyEngine/Utils/Types/UUID.h"

#include "DummyEngine/Utils/Debug/Assert.h"
#include "DummyEngine/Utils/Helpers/StringOperations.h"

namespace DE {
    static std::random_device                 s_RandomDevice;
    static std::mt19937_64                    s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<U64> s_UniformDistribution;

    UUID::UUID(const std::string& str) {
        DE_ASSERT(str.size() == 32, "Wrong str size (", str.size(), ")");
        std::string_view first(str);
        std::string_view second(str);
        first.remove_suffix(16);
        second.remove_prefix(16);
        m_First  = StrToHex(first);
        m_Second = StrToHex(second);
    }
    UUID::operator std::string() const {
        return StrCat(HexToStr(m_First), HexToStr(m_Second));
    }
    std::string UUID::Hex() const {
        return StrCat(HexToStr(m_First), HexToStr(m_Second));
    }

    bool UUID::operator==(const UUID& other) const {
        return m_First == other.m_First && m_Second == other.m_Second;
    }
    bool UUID::operator!=(const UUID& other) const {
        return m_First != other.m_First || m_Second != other.m_Second;
    }
    bool UUID::operator<(const UUID& other) const {
        return (m_First == other.m_First ? m_Second < other.m_Second : m_First < other.m_First);
    }
    bool UUID::operator>(const UUID& other) const {
        return (m_First == other.m_First ? m_Second > other.m_Second : m_First > other.m_First);
    }

    UUID UUID::Generate() {
        UUID res;
        res.m_First  = s_UniformDistribution(s_Engine);
        res.m_Second = s_UniformDistribution(s_Engine);
        return res;
    }
}  // namespace DE
