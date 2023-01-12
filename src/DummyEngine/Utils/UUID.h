#pragma once

#include <iomanip>

namespace DE
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t id) : m_UUID(id) {}
        UUID(const UUID&) = default;

        operator uint64_t() const
        {
            return m_UUID;
        }
        operator std::string() const
        {
            std::stringstream stream;
            stream << std::setfill('0') << std::setw(16) << std::hex << m_UUID;
            return stream.str();
        }
        std::string Hex() const
        {
            std::stringstream stream;
            stream << std::setfill('0') << std::setw(16) << std::hex << m_UUID;
            return stream.str();
        }
        bool operator==(const UUID& other) const
        {
            return (m_UUID == other.m_UUID);
        }
        bool operator!=(const UUID& other) const
        {
            return (m_UUID != other.m_UUID);
        }

    private:
        uint64_t m_UUID;
    };

}  // namespace DE

namespace std
{
    template <typename T>
    struct hash;

    template <>
    struct hash<DE::UUID>
    {
        std::size_t operator()(const DE::UUID& uuid) const
        {
            return (uint64_t)uuid;
        }
    };

}  // namespace std