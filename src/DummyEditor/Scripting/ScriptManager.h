#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"

#include "DummyEditor/Scripting/EditorScripts.h"

namespace DE
{
    using CreateScriptFunc = Ref<Script> (*)();

    class ScriptManager : public Singleton<ScriptManager>
    {
        SINGLETON(ScriptManager)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(Unit, PrepareScripts, (const Path& scene_path));
        S_METHOD_DEF(Unit, ReloadSripts, ());

    private:
        ScriptManager()  = default;
        ~ScriptManager() = default;

        void LoadEditorLibrary();
        void LoadEditorScripts();

        void        LoadLibrary(const std::string& name);
        bool        NeedToCompile(const Path& path);
        std::string AvailableName();

        const std::string kName1 = "ScriptLibrary1";
        const std::string kName2 = "ScriptLibrary2";
        std::vector<Path> m_Scripts;
        std::string       m_LibraryName = kName1;
    };
}  // namespace DE