#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/ScriptInstance.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"

namespace DE
{
    using CreateScriptInstanceFunc = Ref<ScriptInstance> (*)();

    class ScriptManager : public Singleton<ScriptManager>
    {
        SINGLETON(ScriptManager)
    public:
        S_METHOD_DEF(ScriptManager, Unit, Initialize, ());
        S_METHOD_DEF(ScriptManager, Unit, Terminate, ());

        S_METHOD_DEF(ScriptManager, bool, ReloadSripts, ());
        S_METHOD_DEF(ScriptManager, bool, Valid, (UUID id));
        S_METHOD_DEF(ScriptManager, bool, AddScript, (const ScriptAsset& asset));

        S_METHOD_DEF(ScriptManager, Unit, DeleteScript, (UUID id));
        S_METHOD_DEF(ScriptManager, Unit, Modify, (UUID id));
        S_METHOD_DEF(ScriptManager, Unit, Clear, ());

        S_METHOD_DEF(ScriptManager, Ref<ScriptInstance>, CreateScriptInstance, (UUID id));

    private:
        ScriptManager()  = default;
        ~ScriptManager() = default;

        bool SourcesExist() const;
        bool UnModifiedObjectsExist() const;
        bool UpdateFuncTable();

        struct ScriptInfo
        {
            bool modified = true;
            Path path;
        };
        Scope<SharedObject>                                m_ScriptLibrary;
        std::unordered_map<UUID, ScriptInfo>               m_ScriptStates;
        std::unordered_map<UUID, CreateScriptInstanceFunc> m_CreateFuncs;
    };
}  // namespace DE