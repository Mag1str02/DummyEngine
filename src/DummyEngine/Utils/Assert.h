#pragma once

#include <string>

namespace DE {
    void FailAssert(const char* expr_str, const char* file, int line, const std::string& msg);

#if DE_ENABLE_ASSERTS
#define DE_ASSERT(Expr, Msg)                            \
    do {                                                \
        if (!(Expr)) {                                  \
            FailAssert(#Expr, __FILE__, __LINE__, Msg); \
        }                                               \
    } while (false)
#else
#define DE_ASSERT(Expr, Msg)
#endif
}  // namespace DE
