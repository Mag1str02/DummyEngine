#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"
#include "DummyEngine/Core/ResourceManaging/HandleManager.h"

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
        Ref<Script> operator->() { return m_Script; }

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
        S_METHOD_DEF(bool, LibraryLoaded, (const std::string& name));
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