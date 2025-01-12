#pragma once

#include "DummyEngine/Core/ECS/System.h"
#include "DummyEngine/Core/ECS/SystemManager.h"

namespace DummyEngine {

#ifdef ECS_IMPLEMENTATION
    SystemManager::SystemManager(Storage* storage) {
        storage_ = storage;
    }
    void SystemManager::Update(float dt) {
        for (auto& system : systems_) {
            system->Update(dt);
        }
    }
#endif
    template <typename Before, typename After> void SystemManager::AddDependency() {}
    template <typename SystemType> void             SystemManager::AttachSystem(std::shared_ptr<System> system) {
        if (system_ids_.find(std::type_index(typeid(SystemType))) == system_ids_.end()) {
            size_t sz                                        = system_ids_.size();
            system_ids_[std::type_index(typeid(SystemType))] = sz;
            systems_.push_back(system);
            systems_.back()->Bind(storage_);
        }
    }

}  // namespace DummyEngine
