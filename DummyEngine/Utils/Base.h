#pragma once

#if WIN32
#define YAML_CPP_STATIC_DEFINE
#endif
#define DE_SCRIPT_API extern "C" __declspec(dllexport)

#include "DummyEngine/Utils/GLMIncludes.h"
#include "DummyEngine/Utils/CompilerDefines.h"
#include "DummyEngine/Utils/Conversions.h"
#include "DummyEngine/Utils/Types.h"
#include "DummyEngine/Utils/Config.h"
#include "DummyEngine/Utils/Assert.h"
#include "DummyEngine/Utils/UUID.h"
#include "DummyEngine/Utils/Random.h"
#include "DummyEngine/Utils/FileSystem.h"
#include "DummyEngine/Utils/GLDebug.h"
#include "DummyEngine/Utils/Singleton.h"
#include "DummyEngine/Utils/StringOperations.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/ToolBox/Dev/Profiler.h"

namespace DE
{
    template <typename T> using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T> using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
    template <typename T> using WeakRef = std::weak_ptr<T>;

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
#ifndef DE_COMPILER_PATH_HASH
    template <> struct hash<::DE::Path>
    {
        std::size_t operator()(const ::DE::Path& path) const { return hash_value(path); }
    };
#endif
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
