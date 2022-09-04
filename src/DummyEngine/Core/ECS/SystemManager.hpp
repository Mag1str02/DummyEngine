#pragma once

#include "Core/ECS/System.hpp"

namespace DE
{
    using SystemId = int16_t;
    class SystemManager
    {
    private:
        std::unordered_map<SystemId, Ref<System>> m_SystemArray;
        std::unordered_map<std::string, SystemId> m_SystemId;

    public:
        SystemManager() {}
        void Update(double dt)
        {
            DE_PROFILE_SCOPE("System Update");
            for (SystemId i = 0; i < m_SystemArray.size(); ++i)
            {
                m_SystemArray[i]->Update(dt);
            }
        }

        template <typename SystemType> Ref<System> RegisterSystem()
        {
            if (m_SystemId.find(typeid(SystemType).name()) == m_SystemId.end())
            {
                m_SystemArray[m_SystemId.size()] = CreateRef<SystemType>();
                m_SystemId[typeid(SystemType).name()] = m_SystemId.size();
            }
            return m_SystemArray[m_SystemId.size() - 1];
        }
    };
}  // namespace DE