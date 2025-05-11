#pragma once

#include "DummyEngine/Utils/Debug/Assert.h"  // IWYU pragma: export
#include "DummyEngine/Utils/Types/Types.h"   // IWYU pragma: export

#include <functional>     // IWYU pragma: export
#include <queue>          // IWYU pragma: export
#include <typeindex>      // IWYU pragma: export
#include <unordered_map>  // IWYU pragma: export
#include <vector>         // IWYU pragma: export

namespace DummyEngine {

    class Signature;
    class Entity;
    class Storage;
    class System;
    class IComponentArray;
    class ComponentManager;
    class EntityManager;
    class SystemManager;
    template <typename... Components> class StorageView;
    template <typename ComponentType> class ComponentArray;

}  // namespace DummyEngine