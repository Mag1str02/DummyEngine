#pragma once

#include <cinttypes>

namespace DE {

class NotEnoughAvailableEntities : public std::exception {};

using EntityId = int64_t;
using ComponentId = uint16_t;

}  // namespace DE
