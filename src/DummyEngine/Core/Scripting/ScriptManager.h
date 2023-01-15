#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/ScriptInstance.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"

namespace DE
{
    using CreateScriptInstanceFunc = Ref<ScriptInstance> (*)();
    class ScriptManager
    {
    public:
        bool AddScript(const ScriptAsset& asset);
        void DeleteScript(UUID id);
        void Modify(UUID id);
        bool ReloadSripts();
        bool Valid(UUID id);
        void Clear();

        Ref<ScriptInstance>   CreateScriptInstance(UUID id);
        static ScriptManager& Get();

    private:
        static ScriptManager* manager;
        ScriptManager();
        ~ScriptManager();

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