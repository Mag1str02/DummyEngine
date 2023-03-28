#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/Scripting/EditorScripts.h"

namespace DE {
    class ScriptManager : public Singleton<ScriptManager> {
        SINGLETON(ScriptManager)
    public:
        S_METHOD_DEF(Unit, LoadScripts, (const std::vector<ScriptAsset>& scripts));
        S_METHOD_DEF(Unit, UnloadScripts, (const std::vector<ScriptAsset>& scripts));
        S_METHOD_DEF(Unit, ReloadScripts, (const std::vector<ScriptAsset>& scripts, Ref<Scene> scene));
        S_METHOD_DEF(Unit, AttachScripts, (Ref<Scene> scene));

    private:
        using ScriptStates = std::unordered_map<Entity, std::unordered_map<std::string, Script::Field>>;

        ScriptStates SaveSciptStates(Ref<Scene> scene);
        void         RestoreSciptStates(const ScriptStates& states, Ref<Scene> scene);

        void* Clone(const Script::Field& field) const;
        void  Restore(ScriptComponent& script, const std::string& name, const Script::Field& field) const;
        void  Delete(const Script::Field& field) const;

        void LoadEditorLibrary();
        void LoadEditorScripts();

        std::vector<uint32_t>      RecompilationList(const std::vector<ScriptAsset>& scripts);
        std::optional<Path>        CompileSelected(const std::vector<ScriptAsset>& scripts, const std::vector<uint32_t> ids);
        std::optional<std::string> LinkLibrary(const std::vector<ScriptAsset>& scripts);
        bool                       SwapLibrary(const std::string& name);
        bool                       NeedToCompile(const Path& path);
        std::string                AvailableName();

        std::unordered_set<Path> m_CompiledScripts;
        std::string              m_LibraryName;
    };
}  // namespace DE