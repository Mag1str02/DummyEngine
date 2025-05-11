#pragma once

#include <cxxabi.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>

namespace DummyEngine {

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

}  // namespace DummyEngine