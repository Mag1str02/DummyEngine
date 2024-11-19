#pragma once

#include "DummyEngine/Utils/Types/Types.h"
#include "DummyEngine/Utils/Types/UUID.h"

namespace std {
    template <> struct formatter<DE::Vec3> : formatter<std::string> {
        auto format(const DE::Vec3& vec, format_context& ctx) const {
            return formatter<string>::format(std::format("({}, {}, {})", vec.x, vec.y, vec.z), ctx);
        }
    };
    template <> struct formatter<DE::Vec4> : formatter<std::string> {
        auto format(const DE::Vec4& vec, format_context& ctx) const {
            return formatter<string>::format(std::format("({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w), ctx);
        }
    };

    template <> struct formatter<DE::Path> : formatter<std::string> {
        auto format(const DE::Path& path, format_context& ctx) const { return formatter<string>::format(path.string(), ctx); }
    };

    template <> struct formatter<DE::UUID> : formatter<std::string> {
        auto format(const DE::UUID& uuid, format_context& ctx) const { return formatter<string>::format(uuid.Hex(), ctx); }
    };
}  // namespace std