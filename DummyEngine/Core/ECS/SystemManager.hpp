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
    template <typename SystemType> void             SystemManager::RegisterSystem() {
        if (m_SystemId.find(INDEX(SystemType)) == m_SystemId.end()) {
            size_t sz                     = m_SystemId.size();
            m_SystemId[INDEX(SystemType)] = sz;
            m_Systems.push_back(std::make_shared<SystemType>());
            m_Systems.back()->Bind(m_Storage);
        }
    }
}  // namespace DE
