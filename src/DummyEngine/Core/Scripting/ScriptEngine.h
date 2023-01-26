#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"

namespace DE
{
    template <class T> class HandleManager;
    template <class T> class Handle
    {
    public:
        T&       Get();
        const T& Get() const;
        bool     Valid() const;
        void     Destroy();

    private:
        friend class HandleManager<T>;
        HandleManager<T>* m_Manager = nullptr;
        uint32_t          m_Id      = 0;
        uint32_t          m_Gen     = 0;
    };
    template <class T> class HandleManager
    {
    public:
        class Iterator
        {
        public:
            bool operator==(const Iterator& other) const { return m_Manager == other.m_Manager && m_Id == other.m_Id; }
            bool operator!=(const Iterator& other) const { return m_Manager != other.m_Manager || m_Id != other.m_Id; }

            Iterator& operator++()
            {
                do
                {
                    ++m_Id;
                }
                while (m_Id < m_Manager->m_Instances.size() && !m_Manager->m_Instances[m_Id].valid);
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator res = *this;
                do
                {
                    ++m_Id;
                }
                while (m_Id < m_Manager->m_Instances.size() && !m_Manager->m_Instances[m_Id].valid);
                return res;
            }
            Iterator& operator--()
            {
                do
                {
                    --m_Id;
                }
                while (m_Id > 0 && !m_Manager->m_Instances[m_Id].valid);
                return *this;
            }
            Iterator operator--(int)
            {
                Iterator res = *this;
                do
                {
                    --m_Id;
                }
                while (m_Id > 0 && !m_Manager->m_Instances[m_Id].valid);
                return res;
            }
            T&       operator*() { return m_Manager->m_Instances[m_Id].instance; }
            T*       operator->() { return &(m_Manager->m_Instances[m_Id].instance); }
            uint32_t ID() const { return m_Id; }

        private:
            friend class HandleManager;
            Iterator(HandleManager<T>* manager, uint32_t id) : m_Manager(manager), m_Id(id) {}

            HandleManager<T>* m_Manager = nullptr;
            uint32_t          m_Id      = 0;
        };

        Iterator begin()
        {
            for (size_t i = 0; i < m_Instances.size(); ++i)
            {
                if (m_Instances[i].valid)
                {
                    return Iterator(this, i);
                }
            }
            return Iterator(this, m_Instances.size());
        }
        Iterator end() { return Iterator(this, m_Instances.size()); }

        Handle<T> CreateHandle()
        {
            if (m_AvailableIds.empty())
            {
                GenId();
            }
            Handle<T> res;
            res.m_Manager               = this;
            res.m_Id                    = m_AvailableIds.front();
            res.m_Gen                   = ++m_Instances[res.m_Id].gen;
            m_Instances[res.m_Id].valid = true;
            m_AvailableIds.pop_front();
            return res;
        }
        void Destroy(uint32_t id)
        {
            if (m_Instances[id].valid)
            {
                m_Instances[id].valid    = false;
                m_Instances[id].instance = T();
                m_AvailableIds.push_back(id);
            }
        }
        void Clear()
        {
            for (uint32_t i = 0; i < m_Instances.size(); ++i)
            {
                Destroy(i);
            }
        }

    private:
        friend class Handle<T>;

        void GenId()
        {
            m_AvailableIds.push_back(m_AvailableIds.size());
            m_Instances.push_back(Instance());
        }

        struct Instance
        {
            T        instance;
            uint32_t gen   = 0;
            bool     valid = false;
        };

        std::deque<uint32_t>  m_AvailableIds;
        std::vector<Instance> m_Instances;
    };

    template <class T> T&       Handle<T>::Get() { return m_Manager->m_Instances[m_Id].instance; }
    template <class T> const T& Handle<T>::Get() const { return m_Manager->m_Instances[m_Id].instance; }
    template <class T> bool     Handle<T>::Valid() const
    {
        return m_Manager && m_Manager->m_Instances[m_Id].valid && m_Manager->m_Instances[m_Id].gen == m_Gen;
    }
    template <class T> void Handle<T>::Destroy()
    {
        m_Manager->m_AvailableIds.push_back(m_Id);
        m_Manager->m_Instances[m_Id].valid = false;
    }
}  // namespace DE

namespace DE
{

    class ScriptInstance
    {
    public:
        UUID                                          ID() const { return m_Id; }
        std::unordered_map<std::string, ScriptField>& GetFields() { return m_Script->GetFields(); }
        ScriptField                                   GetField(const std::string& field_name) { return m_Script->GetField(field_name); }
        bool                                          Valid() const { return m_Script != nullptr; }
        //! Temporary, should not reveal script
        Ref<Script> Get() { return m_Script; }

    private:
        friend class ScriptEngine;

        Ref<Script> m_Script = nullptr;
        UUID        m_Id;
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
        S_METHOD_DEF(Unit, ClearLibraries, ());

        S_METHOD_DEF(Handle<ScriptInstance>, CreateScript, (UUID id));

    private:
        ScriptEngine()  = default;
        ~ScriptEngine() = default;
        void                     CheckScripts();
        void                     UpdateScriptClasses(Ref<SharedObject> library);
        std::unordered_set<UUID> GetInvalidClasses();

        std::vector<Ref<SharedObject>>        m_Libraries;
        std::unordered_map<UUID, ScriptClass> m_ScriptClasses;
        HandleManager<ScriptInstance>         m_HandleManager;
    };
}  // namespace DE