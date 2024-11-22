#include "UUID.h"

#include "DummyEngine/Utils/Debug/Assert.h"
#include "DummyEngine/Utils/Helpers/StringOperations.h"

#include <random>

namespace DummyEngine {
    static thread_local std::random_device                 gRandomDevice;
    static thread_local std::mt19937_64                    gEngine(gRandomDevice());
    static thread_local std::uniform_int_distribution<U64> gUniformDistribution;

    UUID::UUID(const std::string& str) {
        DE_ASSERT(str.size() == 32, "Wrong str size {}", str.size());
        std::string_view first(str);
        std::string_view second(str);
        first.remove_suffix(16);
        second.remove_prefix(16);
        first_  = StrToHex(first);
        second_ = StrToHex(second);
    }
    UUID::operator std::string() const {
        return std::format("{:032x}{:032x}", first_, second_);
    }
    std::string UUID::Hex() const {
        return std::format("{:032x}{:032x}", first_, second_);
    }

    bool UUID::operator==(const UUID& other) const {
        return first_ == other.first_ && second_ == other.second_;
    }
    bool UUID::operator!=(const UUID& other) const {
        return first_ != other.first_ || second_ != other.second_;
    }
    bool UUID::operator<(const UUID& other) const {
        return (first_ == other.first_ ? second_ < other.second_ : first_ < other.first_);
    }
    bool UUID::operator>(const UUID& other) const {
        return (first_ == other.first_ ? second_ > other.second_ : first_ > other.first_);
    }

    UUID UUID::Generate() {
        UUID res;
        res.first_  = gUniformDistribution(gEngine);
        res.second_ = gUniformDistribution(gEngine);
        return res;
    }
}  // namespace DummyEngine
