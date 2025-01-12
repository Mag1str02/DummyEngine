#pragma once

#include <format>  // IWYU pragma: export

namespace DummyEngine {

    void FailAssert(const char* expr_str, const char* file, int line, const std::string& msg);

}  // namespace DummyEngine

#if DE_ENABLE_ASSERTS
#define DE_ASSERT(Expr, ...)                                                                \
    do {                                                                                    \
        if (!(Expr)) {                                                                      \
            ::DummyEngine::FailAssert(#Expr, __FILE__, __LINE__, std::format(__VA_ARGS__)); \
        }                                                                                   \
    } while (false)
#else
#define DE_ASSERT(Expr, Msg)
#endif