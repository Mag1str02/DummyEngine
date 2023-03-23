#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"
#include "DummyEngine/Core/ResourceManaging/HandleManager.h"

namespace DE
{
    class ScriptComponent
    {
    public:
        ScriptComponent(uint32_t id, uint32_t gen) : m_ID(id), m_Gen(gen) {}
        UUID    ID() const;
        bool    Valid() const;
        void    Destroy();
        Script& operator*();
        Script* operator->();

    private:
        friend class ScriptEngine;

        uint32_t m_ID  = 0;
        uint32_t m_Gen = 0;
    };

    struct ScriptProxy
    {
        Script* m_Script = nullptr;
        UUID    m_Id;
    };

    class ScriptProxyManager
    {
    public:
        class Iterator
        {
        public:
            bool operator==(const Iterator& other) const { return m_Manager == other.m_Manager && m_ID == other.m_ID; }
            bool operator!=(const Iterator& other) const { return m_ID != other.m_ID || m_Manager != other.m_Manager; }

            Iterator& operator++()
            {
                do
                {
                    ++m_ID;
                }
                while (m_ID < m_Manager->m_States.size() && !m_Manager->m_States[m_ID]);
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator res = *this;
                return ++(*this);
            }
            ScriptProxy& operator*() { return m_Manager->m_Proxys[m_ID]; }
            ScriptProxy* operator->() { return &(m_Manager->m_Proxys[m_ID]); }

        private:
            friend class ScriptProxyManager;
            Iterator(ScriptProxyManager* manager, uint32_t id) : m_Manager(manager), m_ID(id) {}

            ScriptProxyManager* m_Manager = nullptr;
            uint32_t            m_ID      = 0;
        };

        Iterator begin()
        {
            int id = 0;
            while (id < m_States.size() && !m_States[id])
            {
                ++id;
            }
            return Iterator(this, id);
        }
        Iterator end() { return Iterator(this, m_States.size()); }

        void Clear()
        {
            m_States.clear();
            m_Proxys.clear();
            m_Generations.clear();
            m_States.clear();
        }
        bool Valid(uint32_t id, uint32_t gen) { return id < m_Proxys.size() && m_States[id] && m_Generations[id] == gen; }
        void Destroy(uint32_t id)
        {
            m_AvailableIds.push_back(id);
            m_States[id] = false;
            LOG_INFO("ScriptProxyManager", "Destroyed handle (", id, ")");
        }
        void Destroy(Iterator it) { Destroy(it.m_ID); }

        ScriptProxy&                  GetProxy(uint32_t id) { return m_Proxys[id]; }
        std::pair<uint32_t, uint32_t> CreateProxy()
        {
            ExtendIfRequired();
            uint32_t id = m_AvailableIds.front();
            m_AvailableIds.pop_front();
            m_States[id]          = true;
            m_Proxys[id].m_Id     = 0;
            m_Proxys[id].m_Script = nullptr;
            LOG_INFO("ScriptProxyManager", "Created handle (", id, ")");
            return {id, ++m_Generations[id]};
        }

    private:
        void ExtendIfRequired()
        {
            if (m_AvailableIds.empty())
            {
                m_AvailableIds.push_back(m_Proxys.size());
                m_Proxys.push_back({nullptr, 0});
                m_States.push_back(false);
                m_Generations.push_back(0);
            }
        }

        std::vector<ScriptProxy> m_Proxys;
        std::vector<uint32_t>    m_Generations;
        std::deque<uint32_t>     m_AvailableIds;
        std::vector<bool>        m_States;
    };

    class ScriptEngine : public Singleton<ScriptEngine>
    {
        SINGLETON(ScriptEngine)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(Unit, AddScript, (const ScriptAsset& asset));
        S_METHOD_DEF(Unit, DeleteScript, (UUID id));
        S_METHOD_DEF(bool, Valid, (UUID id));
        S_METHOD_DEF(Unit, ClearScripts, ());

        S_METHOD_DEF(Unit, AddLibrary, (Ref<SharedObject> library));
        S_METHOD_DEF(Unit, DeleteLibrary, (const std::string& name));
        S_METHOD_DEF(bool, LibraryLoaded, (const std::string& name));
        S_METHOD_DEF(Unit, ClearLibraries, ());

        S_METHOD_DEF(ScriptComponent, CreateScript, (UUID id));
        S_METHOD_DEF(Script*, GetScript, (const ScriptComponent& compoent));
        S_METHOD_DEF(UUID, GetUUID, (const ScriptComponent& compoent));
        S_METHOD_DEF(bool, Valid, (const ScriptComponent& compoent));
        S_METHOD_DEF(Unit, Destroy, (const ScriptComponent& compoent));

    private:
        ScriptEngine()  = default;
        ~ScriptEngine() = default;
        void UpdateScriptClasses(Ref<SharedObject> library);

        std::vector<Ref<SharedObject>>        m_Libraries;
        std::unordered_map<UUID, ScriptClass> m_ScriptClasses;
        ScriptProxyManager                    m_ProxyManager;
    };
}  // namespace DE