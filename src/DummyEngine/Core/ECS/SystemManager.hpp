#pragma once

#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"
#include "DummyEngine/Core/ECS/System.hpp"

namespace DE
{
    using SystemId = int16_t;
    class SystemManager
    {
    private:
        std::unordered_map<SystemId, std::shared_ptr<System>> m_SystemArray;
        std::unordered_map<std::string, SystemId> m_SystemId;
        std::vector<std::vector<SystemId>> m_OrderGraph;
        std::vector<std::vector<SystemId>> m_ProcessingLayers;
        size_t m_MaxLayerWidth;

        SystemManager() {}

        static SystemManager& Get()
        {
            static SystemManager static_manager;
            return static_manager;
        }

        void IUpdate(double dt)
        {
            for (const auto& layer : m_ProcessingLayers)
            {
                for (const auto& system : layer)
                {
                    m_SystemArray[system]->Update(dt);
                }
            }
        }
        template <typename Before, typename After>
        void IAddOrder()
        {
            m_OrderGraph[m_SystemId[typeid(Before).name()]].push_back(m_SystemId[typeid(After).name()]);
        }
        void IClearOrder()
        {
            m_OrderGraph.assign(m_SystemId.size(), std::vector<SystemId>());
        }
        void ICalculateOrder()
        {
            m_ProcessingLayers.clear();
            m_ProcessingLayers.push_back(std::vector<SystemId>());
            for (size_t i = 0; i < m_SystemId.size(); ++i)
            {
                m_ProcessingLayers[0].push_back(i);
            }
            m_MaxLayerWidth = m_SystemId.size();
        }
        template <typename SystemType>
        void IRegisterSystem()
        {
            if (m_SystemId.find(typeid(SystemType).name()) == m_SystemId.end())
            {
                m_SystemArray[m_SystemId.size()] = std::make_shared<SystemType>();
                m_SystemId[typeid(SystemType).name()] = m_SystemId.size();
                m_OrderGraph.push_back(std::vector<SystemId>());
            }
        }

        void ITerminate()
        {
            m_SystemArray.clear();
            m_SystemId.clear();
            m_OrderGraph.clear();
        }

    public:
        static void Update(double dt)
        {
            Get().IUpdate(dt);
        }

        template <typename Before, typename After>
        static void AddOrder()
        {
            Get().IAddOrder<Before, After>();
        }
        static void ClearOrder()
        {
            Get().IClearOrder();
        }
        static void CalculateOrder()
        {
            Get().ICalculateOrder();
        }

        template <typename SystemType>
        static void RegisterSystem()
        {
            Get().IRegisterSystem<SystemType>();
        }

        static void Terminate()
        {
            Get().ITerminate();
        }
    };
}  // namespace DE