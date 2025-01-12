#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstdint>
#include <filesystem>

#define MAX_BONE_INFLUENCE 8
#define MAX_BONES 128

using U8  = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using S8  = int8_t;
using S16 = int16_t;
using S32 = int32_t;
using S64 = int64_t;

namespace DummyEngine {
    namespace fs = std::filesystem;

    using VoidFPtr = void (*)();

    using Mat4       = glm::mat4;
    using Vec4       = glm::vec4;
    using Vec3       = glm::vec3;
    using Vec2       = glm::vec2;
    using IVec4      = glm::ivec4;
    using IVec3      = glm::ivec3;
    using IVec2      = glm::ivec2;
    using Quaternion = glm::quat;
    using Path       = std::filesystem::path;

    template <typename A, typename B> using Pair = std::pair<A, B>;

    // TODO: Move away
    enum class API { None = 0, OpenGL, Vulkan };
    enum class ShaderPartType { None = 0, Vertex, Geometry, Fragment };
    enum class BufferElementType { None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat4 };
    enum class BufferUsage { None = 0, Static, Dynamic };

    struct Vertex3D {
        Vec3  Position                    = Vec3(0);
        Vec3  Normal                      = Vec3(0);
        Vec3  Tangent                     = Vec3(0);
        Vec2  TexCoords                   = Vec2(0);
        S32   Bones[MAX_BONE_INFLUENCE]   = {-1, -1, -1, -1, -1, -1, -1, -1};
        float Weights[MAX_BONE_INFLUENCE] = {0, 0, 0, 0, 0, 0, 0, 0};

        void AddBone(S32 id, float weight);
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
}  // namespace DummyEngine
