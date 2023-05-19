#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#ifdef __GNUC__
#define DE_COMPILER_GCC_VERSION_MAJOR __GNUC__
#else
#define DE_COMPILER_GCC_VERSION_MAJOR 0
#endif

#ifdef __clang_major__
#define DE_COMPILER_CLANG_VERSION_MAJOR __clang_major__
#else
#define DE_COMPILER_CLANG_VERSION_MAJOR 0
#endif

#if DE_COMPILER_GCC_VERSION_MAJOR >= 12 || DE_COMPILER_CLANG_VERSION_MAJOR >= 12
#define DE_COMPILER_PATH_HASH
#endif

#if DE_PLATFORM_WINDOWS
#define YAML_CPP_STATIC_DEFINE
#define DE_SCRIPT_API extern "C" __declspec(dllexport)
#endif

#if DE_PLATFORM_LINUX
#define DE_SCRIPT_API extern "C"
#endif

namespace std {
#ifndef DE_COMPILER_PATH_HASH
    template <> struct hash<::DE::Path> {
        std::size_t operator()(const ::DE::Path& path) const { return hash_value(path); }
    };
#endif
}  // namespace std
