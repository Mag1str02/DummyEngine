#include "DummyEngine/Utils/Debug/Assert.h"

#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Debug/StackTrace.h"

namespace DE {

    extern bool g_EnteredInMain;

    void FailAssert(const char* expr_str, const char* file, int line, const std::string& msg) {
        std::stringstream ss;
        ss << StackTrace();

        ss << "\n"
           << "Assert failed:\t" << msg << "\n"
           << "Expected:\t" << expr_str << "\n"
           << "Source:\t\t" << file << ", line " << line << "\n";
        std::cerr << ss.str() << std::endl;
        if (Logger::Initialized()) {
            LOG_FATAL("Assertion", 0, ss.str());
        }
        exit(1);
    }

}  // namespace DE