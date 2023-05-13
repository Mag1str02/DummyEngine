#pragma once

#include <string>

namespace DE {
    template <typename T> std::string DemangledName() {
        return "Demangler not impemented\n";
    }
    std::string DemangledName(const std::string& name);
}  // namespace DE