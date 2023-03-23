#include "DummyEditor/Scripting/EditorScripts.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class ScriptManager : public Singleton<ScriptManager> {
        SINGLETON(ScriptManager)
    public:
        S_METHOD_DEF(Unit, LoadScripts, (const std::vector<ScriptAsset>& scripts));
        S_METHOD_DEF(Unit, ReloadScripts, (Ref<Scene> scene));
        S_METHOD_DEF(Unit, AttachScripts, (Ref<Scene> scene));

    private:
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