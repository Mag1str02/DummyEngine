#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"

namespace DE
{
    using CreateScriptFunc = Ref<Script> (*)();

    class ScriptEngine : public Singleton<ScriptEngine>
    {
        SINGLETON(ScriptEngine)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(bool, AddScript, (const ScriptAsset& asset));
        S_METHOD_DEF(Unit, DeleteScript, (UUID id));

        S_METHOD_DEF(Unit, Modify, (UUID id));

        S_METHOD_DEF(Unit, Clear, ());

        S_METHOD_DEF(bool, ReloadSripts, ());
        S_METHOD_DEF(bool, Valid, (UUID id));

        S_METHOD_DEF(Ref<Script>, CreateScript, (UUID id));

    private:
        ScriptEngine()  = default;
        ~ScriptEngine() = default;

        bool SourcesExist() const;
        bool UnModifiedObjectsExist() const;
        bool UpdateFuncTable();

        struct ScriptInfo
        {
            bool modified = true;
            Path path;
        };
        Ref<SharedObject>                                  m_ScriptLibrary;
        std::unordered_map<UUID, ScriptInfo>               m_ScriptStates;
        std::unordered_map<UUID, CreateScriptFunc> m_CreateFuncs;
    };
}  // namespace DE