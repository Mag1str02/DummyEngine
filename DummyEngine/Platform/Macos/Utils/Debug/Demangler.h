#pragma once

#include <cxxabi.h>
#include <string>

namespace DummyEngine {
    template <typename T> std::string DemangledName() {
        std::string           res;
        int                   status;
        char*                 realname;
        const std::type_info& ti = typeid(T);

        realname = abi::__cxa_demangle(ti.name(), nullptr, nullptr, &status);
        res      = realname;
        free(realname);
        return res;
    }
    std::string DemangledName(const std::string& name);
}  // namespace DummyEngine