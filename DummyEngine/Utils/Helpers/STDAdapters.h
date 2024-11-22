#pragma once

#include "DummyEngine/Utils/Types/Types.h"
#include "DummyEngine/Utils/Types/UUID.h"

#include <format>

namespace std {
    template <> struct formatter<DummyEngine::Vec3> : formatter<std::string> {
        auto format(const DummyEngine::Vec3& vec, format_context& ctx) const {
            return formatter<string>::format(std::format("({}, {}, {})", vec.x, vec.y, vec.z), ctx);
        }
    };
    template <> struct formatter<DummyEngine::Vec4> : formatter<std::string> {
        auto format(const DummyEngine::Vec4& vec, format_context& ctx) const {
            return formatter<string>::format(std::format("({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w), ctx);
        }
    };

    template <> struct formatter<DummyEngine::Path> : formatter<std::string> {
        auto format(const DummyEngine::Path& path, format_context& ctx) const { return formatter<string>::format(path.string(), ctx); }
    };

    template <> struct formatter<DummyEngine::UUID> : formatter<std::string> {
        auto format(const DummyEngine::UUID& uuid, format_context& ctx) const { return formatter<string>::format(uuid.Hex(), ctx); }
    };
}  // namespace std