#pragma once

#include <cinttypes>

namespace DE {
using ComponentType = std::uint16_t;

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
};
}  // namespace DE
