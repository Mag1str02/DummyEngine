#pragma once

namespace DE {
#ifdef ECS_IMPLEMENTATION
    SystemManager::SystemManager(Storage* storage) {
        m_Storage = storage;
    }
    void SystemManager::Update(float dt) {
        for (auto& system : m_Systems) {
            system->Update(dt);
        }
    }
#endif
    template <typename Before, typename After> void SystemManager::AddDependency() {}
    template <typename SystemType> void             SystemManager::AttachSystem(std::shared_ptr<System> system) {
        if (m_SystemId.find(INDEX(SystemType)) == m_SystemId.end()) {
            size_t sz                     = m_SystemId.size();
            m_SystemId[INDEX(SystemType)] = sz;
            m_Systems.push_back(system);
            m_Systems.back()->Bind(m_Storage);
        }
    }
}  // namespace DE
