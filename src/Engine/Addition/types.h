#pragma once

#include <cinttypes>

namespace DE {

class NotEnoughAvailableEntities : public std::exception {};

using EntityId = int64_t;
using InstanceId = int64_t;
using ReferenceCount = int64_t;
using ComponentId = int16_t;
using SystemId = int16_t;
using ShaderId = unsigned int;
using ShaderProgramId = unsigned int;

}  // namespace DE
