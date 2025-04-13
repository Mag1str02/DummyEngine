#include "DummyEngine/Platform/Macos/Utils/Debug/Demangler.h"

namespace DummyEngine {
    std::string DemangledName(const std::string& name) {
        std::string res;
        int         status;
        char*       realname;

        realname = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
        res      = realname;
        free(realname);
        return res;
    }
}  // namespace DummyEngine