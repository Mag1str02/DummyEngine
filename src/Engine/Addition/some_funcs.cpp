#include "some_funcs.h"

namespace DE {
std::string NormalTypeName(const std::string& s_name) {
    for (size_t i = 1; i < s_name.size(); ++i) {
        if ('0' > s_name[i] || s_name[i] > '9') {
            return s_name.substr(i, s_name.size() - i);
        }
    }
    return s_name;
}

}  // namespace DE