#pragma once

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