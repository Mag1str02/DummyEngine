#include <iostream>

#include "DummyEngine/Addition/Assert.h"

namespace DE
{
    void DEAssert(const char* expr_str, bool expr, const char* file, int line, std::string msg)
    {
        if (!expr)
        {
            std::cerr << "Assert failed:\t" << msg << "\n"
                      << "Expected:\t" << expr_str << "\n"
                      << "Source:\t\t" << file << ", line " << line << "\n";
            abort();
        }
    }

}  // namespace DE