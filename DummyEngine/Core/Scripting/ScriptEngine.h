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
        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent& other);
        ScriptComponent(ScriptComponent&& other);
        ScriptComponent& operator=(const ScriptComponent& other);
        ScriptComponent& operator=(ScriptComponent&& other);
        ~ScriptComponent();

        UUID    ID() const;
        bool    Valid() const;
        Script& operator*();
        Script* operator->();

    private:
        ScriptComponent(U32 id);
        friend class ScriptEngine;

        U32 m_ID = UINT32_MAX;
    };

    struct ScriptProxy {
        Script* m_Script   = nullptr;
        U32     m_RefCount = 0;
        UUID    m_ScriptID;
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
        void Destroy(U32 id);
        void Destroy(Iterator it);

        ScriptProxy& GetProxy(U32 id);
        U32          CreateProxy();

    private:
        void ExtendIfRequired();

        std::vector<ScriptProxy> m_Proxys;
        std::deque<U32>          m_AvailableIds;
        std::vector<bool>        m_States;
    };

    class ScriptEngine : public Singleton<ScriptEngine> {
        LOGGER_AUTHOR(ScriptEngine)
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

    private:
        S_METHOD_DEF(Unit, IncreaseRefCount, (U32 id));
        S_METHOD_DEF(Unit, DecreaseRefCount, (U32 id));
        void Destroy(U32 id);

        friend class ScriptComponent;

        void UpdateScriptClasses(Ref<SharedObject> library);

        std::vector<Ref<SharedObject>>        m_Libraries;
        std::unordered_map<UUID, ScriptClass> m_ScriptClasses;
        ScriptProxyManager                    m_ProxyManager;
    };
}  // namespace DE