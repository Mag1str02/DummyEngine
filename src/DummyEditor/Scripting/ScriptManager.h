#include "DummyEditor/Scripting/EditorScripts.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptClass.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    using CreateScriptFunc = Ref<Script> (*)();

    class ScriptManager : public Singleton<ScriptManager> {
        SINGLETON(ScriptManager)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(Unit, ReloadScripts, (Ref<Scene> scene));

    private:
        ScriptManager()  = default;
        ~ScriptManager() = default;

        void LoadEditorLibrary();
        void LoadEditorScripts();

        void        LoadLibrary(const std::string& name);
        bool        NeedToCompile(const Path& path);
        std::string AvailableName();

        std::unordered_set<Path> m_CompiledScripts;
        std::string              m_LibraryName;
    };
}  // namespace DE