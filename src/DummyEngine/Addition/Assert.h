#pragma once

#include "DummyEngine/Addition/CompileConfig.h"

namespace DE
{
    void DEAssert(const char* expr_str, bool expr, const char* file, int line, std::string msg);

#if DE_ENABLE_ASSERTS
#define DE_ASSERT(Expr, Msg) DEAssert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#define DE_ASSERT(Expr, Msg)
#endif
}  // namespace DE
