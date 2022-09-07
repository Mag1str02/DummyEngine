#pragma once

#if WIN32
#define YAML_CPP_STATIC_DEFINE
#endif

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/glm.hpp>

#include <YAML-CPP/yaml.h>

#include <memory>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <set>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <cstdint>
#include <queue>
#include <stack>
#include <random>
#include <cxxabi.h>

#include "Addition/Conversions.h"
#include "Addition/Types.h"
#include "Addition/Config.h"
#include "Addition/Assert.h"
#include "Addition/UUID.h"
#include "Addition/Random.h"
#include "Addition/FileDialogs.h"
#include "Addition/GLDebug.h"

#include "ToolBox/Dev/Logger.h"
#include "ToolBox/Dev/Profiler.h"

namespace DE
{
    template <typename T> using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

    template <typename T> using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

    namespace fs = std::filesystem;

    template <typename T> std::string DemangleName()
    {
        std::string           res;
        int                   status;
        char*                 realname;
        const std::type_info& ti = typeid(T);

        realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
        res      = realname;
        free(realname);
        return res;
    }

}  // namespace DE
namespace std
{
    template <> struct hash<std::filesystem::path>
    {
        std::size_t operator()(const std::filesystem::path& path) const { return hash_value(path); }
    };
    struct pair_hash
    {
        template <class T1, class T2> std::size_t operator()(const std::pair<T1, T2>& p) const
        {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);

            return h1 ^ h2;
        }
    };
}  // namespace std
