#pragma once

#include "DummyEngine/Utils/Base/GLMIncludes.h"
#include "DummyEngine/Utils/Base/STDIncludes.h"

using U8  = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using S8  = int8_t;
using S16 = int16_t;
using S32 = int32_t;
using S64 = int64_t;

namespace DE {
    namespace fs = std::filesystem;

    using VoidFPtr = void (*)();

    using Mat4 = glm::mat4;
    using Vec4 = glm::vec4;
    using Vec3 = glm::vec3;
    using Vec2 = glm::vec2;
    using Path = std::filesystem::path;

    template <typename A, typename B> using Pair = std::pair<A, B>;

    // TODO: Move away
    enum class API { None = 0, OpenGL, Vulkan };
    enum class ShaderPartType { None = 0, Vertex, Geometry, Fragment };
    enum class BufferElementType { None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat4 };
    enum class BufferUsage { None = 0, Static, Dynamic };

    struct Vertex3D {
        Vec3 position;
        Vec3 normal;
        Vec2 tex_coords;
    };

    template <typename T> using Scope   = std::unique_ptr<T>;
    template <typename T> using Ref     = std::shared_ptr<T>;
    template <typename T> using WeakRef = std::weak_ptr<T>;

    template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    struct PairHash {
        template <class T1, class T2> std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);

            return h1 ^ h2;
        }
    };
}  // namespace DE