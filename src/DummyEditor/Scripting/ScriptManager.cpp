#include "DummyEditor/Scripting/ScriptManager.h"

#include "DummyEditor/Scripting/Compiler.h"
#include "DummyEditor/Scripting/EditorScripts.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"

namespace DE {
    Path PathToCompiledScript(Path path) {
        return Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / (fs::relative(path, Config::GetPath(DE_CFG_SCRIPT_PATH)).string() + ".o");
    }

    SINGLETON_BASE(ScriptManager);

    Unit ScriptManager::Initialize() {
        DE_ASSERT(!s_Instance, "Double ScriptManager initialization");
        s_Instance = new ScriptManager();
        DE_ASSERT(s_Instance, "Failed to allocate memory for ScriptManager");
        s_Instance->IInitialize();
        LOG_INFO("ScriptManager", "ScriptManager initialized");

        return Unit();
    }
    Unit ScriptManager::IInitialize() {
        LoadEditorLibrary();
        LoadEditorScripts();
        return Unit();
    }
    Unit ScriptManager::Terminate() {
        s_Instance->ITerminate();
        delete s_Instance;
        return Unit();
    }
    Unit ScriptManager::ITerminate() {
        return Unit();
    }

    // S_METHOD_IMPL(ScriptManager, Unit, PrepareScripts, (Ref<Scene> scene), (scene)) {
    //     LOG_INFO("ScriptManager", "Preparing scripts...");
    //     ScriptEngine::ClearScripts();
    //     auto assets = SceneLoader::GetScriptAssets(scene_path);
    //     for (const auto& script : assets) {
    //         scripts.push_back(script.path);
    //     }
    //     ReloadSripts();
    //     LoadEditorScripts();

    //     LOG_INFO("ScriptManager", "Scripts prepared");
    //     return Unit();
    // }
    S_METHOD_IMPL(ScriptManager, Unit, ReloadScripts, (Ref<Scene> scene), (scene)) {
        const auto& scripts = scene->GetScripts();
        if (scripts.empty()) {
            return Unit();
        }

        bool                  need_recompile = false;
        std::vector<uint32_t> recompile_ids;

        //*Find scripts to recompile
        {
            for (size_t i = 0; i < scripts.size(); ++i) {
                DE_ASSERT(fs::exists(scripts[i]), StrCat("Failed to find script in library: ", scripts[i].string()));
                if (NeedToCompile(scripts[i])) {
                    recompile_ids.push_back(i);
                    need_recompile = true;
                }
            }
            if (!need_recompile && !m_LibraryName.empty()) {
                return Unit();
            }
        }

        //*Compile sources
        {
            for (auto id : recompile_ids) {
                bool compilation_success = Compiler::Compile(scripts[id], PathToCompiledScript(scripts[id]));
                DE_ASSERT(compilation_success, StrCat("Failed to compile: ", scripts[id].string()));
            }
        }

        //*Swap library
        {
            std::vector<Path> compiled_sources;
            for (const auto& path : scripts) {
                compiled_sources.push_back(PathToCompiledScript(path));
            }
            std::string new_name     = AvailableName();
            bool        link_success = Compiler::Link(compiled_sources, Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), new_name);
            DE_ASSERT(link_success, "Failed to link library.");

            LoadLibrary(new_name);
        }

        //*Attach scripts to scene
        {
            for (auto entity : scene->View<ScriptComponent>()) {
                auto& script_component = entity.Get<ScriptComponent>();
                if (script_component.Valid()) {
                    script_component->AttachToScene(scene.get(), entity);
                }
            }
        }

        LOG_INFO("ScriptManager", "Reloaded script for scene (", scene->GetName(), ")");
        return Unit();
    }

    void ScriptManager::LoadEditorLibrary() {
        Ref<SharedObject> library               = CreateRef<SharedObject>();
        bool              editor_library_loaded = library->Load(Config::GetPath(DE_CFG_EXECUTABLE_PATH), DE_EDITOR_LIBRARY_NAME);
        DE_ASSERT(editor_library_loaded, "Failed to load editor library.");
        ScriptEngine::AddLibrary(library);
    }
    void ScriptManager::LoadEditorScripts() {
        for (const auto& asset : g_EditorScriptAssets) {
            ScriptEngine::AddScript(asset);
        }
    }

    void ScriptManager::LoadLibrary(const std::string& name) {
        Ref<SharedObject> library      = CreateRef<SharedObject>();
        bool              load_success = library->Load(Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), name);
        DE_ASSERT(load_success, "Failed to load library.");

        ScriptEngine::DeleteLibrary(m_LibraryName);
        ScriptEngine::AddLibrary(library);
        m_LibraryName = name;
    }
    bool ScriptManager::NeedToCompile(const Path& path) {
        if (!m_CompiledScripts.contains(path)) {
            return true;
        }
        Path object = PathToCompiledScript(path);
        if (!fs::exists(object)) {
            return true;
        }
        return fs::last_write_time(path) > fs::last_write_time(object);
    }
    std::string ScriptManager::AvailableName() {
        // TODO: Generate available name properly
        if (m_LibraryName.empty()) {
            return "ScriptLibrary0";
        }
        return (m_LibraryName.back() == '0' ? "ScriptLibrary1" : "ScriptLibrary0");
    }
}  // namespace DE