#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/ResourceManaging/HandleManager.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class ScriptComponent {
    public:
        ScriptComponent(U32 id, U32 gen) : m_ID(id), m_Gen(gen) {}
        UUID    ID() const;
        bool    Valid() const;
        bool    Loaded() const;
        void    Destroy();
        Script& operator*();
        Script* operator->();

    private:
        friend class ScriptEngine;

        U32 m_ID  = 0;
        U32 m_Gen = 0;
    };

    struct ScriptProxy {
        Script* m_Script = nullptr;
        UUID    m_ID;
    };

    class ScriptProxyManager {
    public:
        class Iterator {
        public:
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;

            Iterator&    operator++();
            Iterator     operator++(int);
            ScriptProxy& operator*();
            ScriptProxy* operator->();

        private:
            friend class ScriptProxyManager;
            Iterator(ScriptProxyManager* manager, U32 id);

            ScriptProxyManager* m_Manager = nullptr;
            U32                 m_ID      = 0;
        };

        Iterator begin();
        Iterator end();

        void Clear();
        bool Valid(U32 id, U32 gen);
        void Destroy(U32 id);
        void Destroy(Iterator it);

        ScriptProxy&        GetProxy(U32 id);
        std::pair<U32, U32> CreateProxy();

    private:
        void ExtendIfRequired();

        std::vector<ScriptProxy> m_Proxys;
        std::vector<U32>         m_Generations;
        std::deque<U32>          m_AvailableIds;
        std::vector<bool>        m_States;
    };

    class ScriptEngine : public Singleton<ScriptEngine> {
        SINGLETON(ScriptEngine)
    public:
        S_METHOD_DEF(bool, AddScript, (UUID id));
        S_METHOD_DEF(bool, DeleteScript, (UUID id));
        S_METHOD_DEF(bool, ValidScript, (UUID id));
        S_METHOD_DEF(Unit, ClearScripts, ());

        S_METHOD_DEF(Unit, AddLibrary, (Ref<SharedObject> library));
        S_METHOD_DEF(Unit, DeleteLibrary, (const std::string& name));
        S_METHOD_DEF(bool, LibraryLoaded, (const std::string& name));
        S_METHOD_DEF(Unit, ClearLibraries, ());

        S_METHOD_DEF(ScriptComponent, CreateScript, (UUID id));
        S_METHOD_DEF(Script*, GetScript, (const ScriptComponent& component));
        S_METHOD_DEF(UUID, GetUUID, (const ScriptComponent& component));
        S_METHOD_DEF(bool, Valid, (const ScriptComponent& component));
        S_METHOD_DEF(bool, Loaded, (const ScriptComponent& component));
        S_METHOD_DEF(Unit, Destroy, (const ScriptComponent& component));

    private:
        void UpdateScriptClasses(Ref<SharedObject> library);

        std::vector<Ref<SharedObject>>        m_Libraries;
        std::unordered_map<UUID, ScriptClass> m_ScriptClasses;
        ScriptProxyManager                    m_ProxyManager;
    };
}  // namespace DE