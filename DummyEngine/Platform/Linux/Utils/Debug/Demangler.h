#pragma once

#include <cxxabi.h>

#include <string>

namespace DE {
    template <typename T> std::string DemangledName() {
        std::string           res;
        int                   status;
        char*                 realname;
        const std::type_info& ti = typeid(T);

        realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
        res      = realname;
        free(realname);
        return res;
    }
    std::string DemangledName(const std::string& name);
}  // namespace DE