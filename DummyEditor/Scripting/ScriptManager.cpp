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
        std::vector<uint32_t> recompile_ids = RecompilationList(scripts);
        if (!recompile_ids.empty()) {
            auto failed_file = CompileSelected(scripts, recompile_ids);
            DE_ASSERT(!failed_file.has_value(), StrCat("Failed to compile source file (", failed_file.value().string(), ")"));
        }
        auto new_library_name = LinkLibrary(scripts);
        DE_ASSERT(new_library_name.has_value(), StrCat("Failed to link library (", new_library_name.value(), ")"));
        auto swapped = SwapLibrary(new_library_name.value());
        DE_ASSERT(swapped, StrCat("Failed to load library (", new_library_name.value(), ")"));

        for (const auto& script : scripts) {
            ScriptEngine::AddScript(script.id);
        }
        LOG_INFO("ScriptManager", "Loaded script for scene");
        return Unit();
    }
    S_METHOD_IMPL(Unit, UnloadScripts, (const std::vector<ScriptAsset>& scripts), (scripts)) {
        for (const auto& script : scripts) {
            ScriptEngine::DeleteScript(script.id);
        }
        if (!m_LibraryName.empty()) {
            ScriptEngine::DeleteLibrary(m_LibraryName);
            m_LibraryName.clear();
        }
        LOG_INFO("ScriptManager", "Unloaded scripts");
        return Unit();
    }
    S_METHOD_IMPL(Unit, ReloadScripts, (const std::vector<ScriptAsset>& scripts, Ref<Scene> scene), (scripts, scene)) {
        if (scripts.empty()) {
            return Unit();
        }
        std::vector<uint32_t> recompile_ids = RecompilationList(scripts);
        if (recompile_ids.empty()) {
            return Unit();
        }

        auto states = SaveSciptStates(scene);

        auto failed_file = CompileSelected(scripts, recompile_ids);
        DE_ASSERT(!failed_file.has_value(), StrCat("Failed to compile source file (", failed_file.value().string(), ")"));
        auto new_library_name = LinkLibrary(scripts);
        DE_ASSERT(new_library_name.has_value(), StrCat("Failed to link library (", new_library_name.value(), ")"));
        auto swapped = SwapLibrary(new_library_name.value());
        DE_ASSERT(swapped, StrCat("Failed to load library (", new_library_name.value(), ")"));

        RestoreSciptStates(states, scene);
        AttachScripts(scene);

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

    ScriptManager::ScriptStates ScriptManager::SaveSciptStates(Ref<Scene> scene) {
        ScriptStates states;
        for (auto entity : scene->View<ScriptComponent>()) {
            auto& script_component = entity.Get<ScriptComponent>();
            if (script_component.Valid()) {
                for (auto [name, field] : *script_component) {
                    states[entity][name.get()] = Script::Field(field.GetType(), Clone(field));
                }
            }
        }
        return states;
    }
    void ScriptManager::RestoreSciptStates(const ScriptStates& states, Ref<Scene> scene) {
        for (auto it = states.begin(); it != states.end(); ++it) {
            auto        entity = it->first;
            const auto& state  = it->second;
            auto&       script = entity.Get<ScriptComponent>();
            if (script.Valid() && !script->AttachedToScene()) {
                for (const auto& [name, field] : state) {
                    Restore(script, name, field);
                    Delete(field);
                }
            }
        }
    }

    void* ScriptManager::Clone(const Script::Field& field) const {
        switch (field.GetType()) {
            case ScriptFieldType::Float: return new float(field.Get<float>());
            case ScriptFieldType::Double: return new double(field.Get<double>());
            case ScriptFieldType::Bool: return new bool(field.Get<bool>());
            case ScriptFieldType::String: return new std::string(field.Get<std::string>());
            case ScriptFieldType::S32: return new int32_t(field.Get<int32_t>());
            case ScriptFieldType::S64: return new int64_t(field.Get<int64_t>());
            case ScriptFieldType::U32: return new uint32_t(field.Get<uint32_t>());
            case ScriptFieldType::U64: return new uint64_t(field.Get<uint64_t>());
            case ScriptFieldType::Vec2: return new Vec2(field.Get<Vec2>());
            case ScriptFieldType::Vec3: return new Vec3(field.Get<Vec3>());
            case ScriptFieldType::Vec4: return new Vec4(field.Get<Vec4>());
            default: return nullptr;
        }
    }
    void ScriptManager::Restore(ScriptComponent& script, const std::string& name, const Script::Field& field) const {
        if (script->HasField(name) && script->GetFieldType(name) == field.GetType()) {
            switch (field.GetType()) {
                case ScriptFieldType::Float: script->GetField<float>(name) = field.Get<float>(); break;
                case ScriptFieldType::Double: script->GetField<double>(name) = field.Get<double>(); break;
                case ScriptFieldType::Bool: script->GetField<bool>(name) = field.Get<bool>(); break;
                case ScriptFieldType::String: script->GetField<std::string>(name) = field.Get<std::string>(); break;
                case ScriptFieldType::S32: script->GetField<int32_t>(name) = field.Get<int32_t>(); break;
                case ScriptFieldType::S64: script->GetField<int64_t>(name) = field.Get<int64_t>(); break;
                case ScriptFieldType::U32: script->GetField<uint32_t>(name) = field.Get<uint32_t>(); break;
                case ScriptFieldType::U64: script->GetField<uint64_t>(name) = field.Get<uint64_t>(); break;
                case ScriptFieldType::Vec2: script->GetField<Vec2>(name) = field.Get<Vec2>(); break;
                case ScriptFieldType::Vec3: script->GetField<Vec3>(name) = field.Get<Vec3>(); break;
                case ScriptFieldType::Vec4: script->GetField<Vec4>(name) = field.Get<Vec4>(); break;
                default: break;
            }
        }
    }
    void ScriptManager::Delete(const Script::Field& field) const {
        switch (field.GetType()) {
            case ScriptFieldType::Float: delete &field.Get<float>(); break;
            case ScriptFieldType::Double: delete &field.Get<double>(); break;
            case ScriptFieldType::Bool: delete &field.Get<bool>(); break;
            case ScriptFieldType::String: delete &field.Get<std::string>(); break;
            case ScriptFieldType::S32: delete &field.Get<int32_t>(); break;
            case ScriptFieldType::S64: delete &field.Get<int64_t>(); break;
            case ScriptFieldType::U32: delete &field.Get<uint32_t>(); break;
            case ScriptFieldType::U64: delete &field.Get<uint64_t>(); break;
            case ScriptFieldType::Vec2: delete &field.Get<Vec2>(); break;
            case ScriptFieldType::Vec3: delete &field.Get<Vec3>(); break;
            case ScriptFieldType::Vec4: delete &field.Get<Vec4>(); break;
            default: break;
        }
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