#include "DummyEditor/Scripting/ScriptManager.h"

#include "DummyEditor/Scripting/Compiler.h"
#include "DummyEditor/Scripting/EditorScripts.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"

namespace DE {
    Path PathToCompiledScript(Path path) {
        return Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / (fs::relative(path, Config::GetPath(DE_CFG_SCRIPT_PATH)).string() + ".o");
    }

    SINGLETON_BASE(ScriptManager);
    S_INITIALIZE() {
        LoadEditorLibrary();
        LoadEditorScripts();
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(Unit, LoadScripts, (const std::vector<ScriptAsset>& scripts), (scripts)) {
        if (scripts.empty()) {
            return Unit();
        }
        for (const auto& script : scripts) {
            ScriptEngine::AddScript(script.id);
        }
        std::vector<uint32_t> recompile_ids = RecompilationList(scripts);
        if (!recompile_ids.empty()) {
            auto failed_file = CompileSelected(scripts, recompile_ids);
            DE_ASSERT(!failed_file.has_value(), StrCat("Failed to compile source file (", failed_file.value().string(), ")"));
            auto new_library_name = LinkLibrary(scripts);
            DE_ASSERT(new_library_name.has_value(), StrCat("Failed to link library (", new_library_name.value(), ")"));
            auto swapped = SwapLibrary(new_library_name.value());
            DE_ASSERT(swapped, StrCat("Failed to load library (", new_library_name.value(), ")"));
        }
        LOG_INFO("ScriptManager", "Loaded script for scene");
        return Unit();
    }
    S_METHOD_IMPL(Unit, AttachScripts, (Ref<Scene> scene), (scene)) {
        for (auto entity : scene->View<ScriptComponent>()) {
            auto& script_component = entity.Get<ScriptComponent>();
            if (script_component.Valid()) {
                script_component->AttachToScene(scene, entity);
            }
        }
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
            AssetManager::AddScriptAsset(asset);
            ScriptEngine::AddScript(asset.id);
        }
    }

    std::vector<uint32_t> ScriptManager::RecompilationList(const std::vector<ScriptAsset>& scripts) {
        std::vector<uint32_t> recompile_ids;
        for (size_t i = 0; i < scripts.size(); ++i) {
            DE_ASSERT(fs::exists(scripts[i].path), StrCat("Failed to find script source file(", scripts[i].path.string(), ")"));
            if (NeedToCompile(scripts[i].path)) {
                recompile_ids.push_back(i);
            }
        }
        return recompile_ids;
    }
    std::optional<Path> ScriptManager::CompileSelected(const std::vector<ScriptAsset>& scripts, const std::vector<uint32_t> ids) {
        for (auto id : ids) {
            if (!Compiler::Compile(scripts[id].path, PathToCompiledScript(scripts[id].path))) {
                return scripts[id].path;
            }
            m_CompiledScripts.insert(scripts[id].path);
        }
        return {};
    }
    std::optional<std::string> ScriptManager::LinkLibrary(const std::vector<ScriptAsset>& scripts) {
        std::vector<Path> compiled_sources;
        for (const auto& script : scripts) {
            compiled_sources.push_back(PathToCompiledScript(script.path));
        }
        std::string new_name = AvailableName();
        return (Compiler::Link(compiled_sources, Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), new_name) ? new_name : std::optional<std::string>());
    }
    bool ScriptManager::SwapLibrary(const std::string& name) {
        Ref<SharedObject> library = CreateRef<SharedObject>();
        if (!library->Load(Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), name)) {
            return false;
        }
        if (!m_LibraryName.empty()) {
            ScriptEngine::DeleteLibrary(m_LibraryName);
        }
        ScriptEngine::AddLibrary(library);
        m_LibraryName = name;
        return true;
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