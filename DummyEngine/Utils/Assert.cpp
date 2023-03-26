#include "DummyEngine/Utils/STDIncludes.h"
#include "DummyEngine/Utils/Assert.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"

namespace DE
{
    void DEAssert(const char* expr_str, bool expr, const char* file, int line, std::string msg)
    {
        if (!expr)
        {
            std::stringstream ss;
            ss << "Assert failed:\t" << msg << "\n"
               << "Expected:\t" << expr_str << "\n"
               << "Source:\t\t" << file << ", line " << line << "\n";
            LOG_FATAL(ss.str(), "Assertion");
            std::cerr << ss.str() << std::endl;
            abort();
        }
    }

}  // namespace DE