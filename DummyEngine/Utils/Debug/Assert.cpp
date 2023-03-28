#include "DummyEngine/Utils/Debug/Assert.h"

#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Debug/StackTrace.h"

namespace DE {

    void FailAssert(const char* expr_str, const char* file, int line, const std::string& msg) {
        std::stringstream ss = StackTrace();
        ss << "\n"
           << "Assert failed:\t" << msg << "\n"
           << "Expected:\t" << expr_str << "\n"
           << "Source:\t\t" << file << ", line " << line << "\n";

        LOG_FATAL("Assertion", 0, ss.str());
        std::cerr << ss.str() << std::endl;
        abort();
    }

}  // namespace DE