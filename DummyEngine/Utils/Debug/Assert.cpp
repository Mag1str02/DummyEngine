#include "Assert.h"

#include "DummyEngine/Utils/Debug/Logger.h"
#include "DummyEngine/Utils/Debug/StackTrace.h"

#include <iostream>
#include <sstream>

namespace DummyEngine {

    extern bool gEnteredInMain;

    void FailAssert(const char* expr_str, const char* file, int line, const std::string& msg) {
        std::stringstream ss;
        ss << StackTrace();

        ss << "\n"
           << "Assert failed:\t" << msg << "\n"
           << "Expected:\t" << expr_str << "\n"
           << "Source:\t\t" << file << ", line " << line << "\n";
        std::cerr << ss.str() << std::endl;
        if (Logger::Initialized()) {
            LOG_FATAL_AS("Assertion", "{}", ss.str());
        }
        abort();
    }

}  // namespace DummyEngine