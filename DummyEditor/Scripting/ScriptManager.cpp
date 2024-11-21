#include "ScriptManager.h"

#include "DummyEditor/Scripting/Compiler.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"

namespace DummyEngine {
    Path PathToCompiledScript(Path path) {
        return Config::Get().ScriptCachePath / (fs::relative(path, Config::Get().ScriptPath).string() + ".o");
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

    S_METHOD_IMPL(bool, LoadScripts, (const std::vector<ScriptAsset>& scripts), (scripts)) {
        if (scripts.empty()) {
            return true;
        }
        std::vector<U32> recompile_ids = RecompilationList(scripts);
        if (!recompile_ids.empty()) {
            auto failed_file = CompileSelected(scripts, recompile_ids);
            if (failed_file.has_value()) {
                LOG_WARNING("Failed to compile source file {}", failed_file.value().string());
                return false;
            }
        };

        auto new_library_name = LinkLibrary(scripts);
        if (!new_library_name.has_value()) {
            LOG_WARNING("Failed to link library {}", new_library_name.value());
            return false;
        }
        auto swapped = SwapLibrary(new_library_name.value());
        if (!swapped) {
            LOG_WARNING("Failed to load library {}", new_library_name.value());
            return false;
        }

        for (const auto& script : scripts) {
            ScriptEngine::AddScript(script.ID);
        }
        LOG_INFO("Loaded script for scene");
        return true;
    }
    S_METHOD_IMPL(Unit, UnloadScripts, (const std::vector<ScriptAsset>& scripts), (scripts)) {
        for (const auto& script : scripts) {
            ScriptEngine::DeleteScript(script.ID);
        }
        if (!library_name_.empty()) {
            ScriptEngine::DeleteLibrary(library_name_);
            library_name_.clear();
        }
        LOG_INFO("Unloaded scripts");
        return Unit();
    }
    S_METHOD_IMPL(bool, ReloadScripts, (const std::vector<ScriptAsset>& scripts, Ref<Scene> scene), (scripts, scene)) {
        if (scripts.empty()) {
            return true;
        }
        std::vector<U32> recompile_ids = RecompilationList(scripts);
        if (recompile_ids.empty()) {
            return true;
        }

        auto states = SaveSciptStates(scene);

        auto failed_file = CompileSelected(scripts, recompile_ids);
        if (failed_file.has_value()) {
            LOG_WARNING("Failed to compile source file {}", failed_file.value().string());
            return false;
        }
        auto new_library_name = LinkLibrary(scripts);
        if (!new_library_name.has_value()) {
            LOG_WARNING("Failed to link library {}", new_library_name.value());
            return false;
        }
        auto swapped = SwapLibrary(new_library_name.value());
        if (!swapped) {
            LOG_WARNING("Failed to load library {}", new_library_name.value());
            return false;
        }

        RestoreSciptStates(states);
        AttachScripts(scene);

        return true;
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
    S_METHOD_IMPL(UUID, EditorScript, (const std::string& name), (name)) {
        DE_ASSERT(editor_script_name_to_id_.contains(name), "There is no editor script with name {}", name);
        return editor_script_name_to_id_.at(name);
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
    void ScriptManager::RestoreSciptStates(const ScriptStates& states) {
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
            case ScriptFieldType::S32: return new S32(field.Get<S32>());
            case ScriptFieldType::S64: return new S64(field.Get<S64>());
            case ScriptFieldType::U32: return new U32(field.Get<U32>());
            case ScriptFieldType::U64: return new U64(field.Get<U64>());
            case ScriptFieldType::Vec2: return new Vec2(field.Get<Vec2>());
            case ScriptFieldType::Vec3: return new Vec3(field.Get<Vec3>());
            case ScriptFieldType::Vec4: return new Vec4(field.Get<Vec4>());
            case ScriptFieldType::Entity: return new Entity(field.Get<Entity>());
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
                case ScriptFieldType::S32: script->GetField<S32>(name) = field.Get<S32>(); break;
                case ScriptFieldType::S64: script->GetField<S64>(name) = field.Get<S64>(); break;
                case ScriptFieldType::U32: script->GetField<U32>(name) = field.Get<U32>(); break;
                case ScriptFieldType::U64: script->GetField<U64>(name) = field.Get<U64>(); break;
                case ScriptFieldType::Vec2: script->GetField<Vec2>(name) = field.Get<Vec2>(); break;
                case ScriptFieldType::Vec3: script->GetField<Vec3>(name) = field.Get<Vec3>(); break;
                case ScriptFieldType::Vec4: script->GetField<Vec4>(name) = field.Get<Vec4>(); break;
                case ScriptFieldType::Entity: script->GetField<Entity>(name) = field.Get<Entity>(); break;
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
            case ScriptFieldType::S32: delete &field.Get<S32>(); break;
            case ScriptFieldType::S64: delete &field.Get<S64>(); break;
            case ScriptFieldType::U32: delete &field.Get<U32>(); break;
            case ScriptFieldType::U64: delete &field.Get<U64>(); break;
            case ScriptFieldType::Vec2: delete &field.Get<Vec2>(); break;
            case ScriptFieldType::Vec3: delete &field.Get<Vec3>(); break;
            case ScriptFieldType::Vec4: delete &field.Get<Vec4>(); break;
            case ScriptFieldType::Entity: delete &field.Get<Entity>(); break;
            default: break;
        }
    }

    void ScriptManager::LoadEditorLibrary() {
        Ref<SharedObject> library               = CreateRef<SharedObject>();
        bool              editor_library_loaded = library->Load(Config::Get().ExecutablePath, DE_EDITOR_LIBRARY_NAME);
        DE_ASSERT(editor_library_loaded, "Failed to load editor library {}", DE_EDITOR_LIBRARY_NAME);
        ScriptEngine::AddLibrary(library);
    }
    void ScriptManager::LoadEditorScripts() {
        for (const auto& asset : editor_script_assets_) {
            AssetManager::AddScriptAsset(asset);
            ScriptEngine::AddScript(asset.ID);
        }
    }

    std::vector<U32> ScriptManager::RecompilationList(const std::vector<ScriptAsset>& scripts) {
        std::vector<U32> recompile_ids;
        for (size_t i = 0; i < scripts.size(); ++i) {
            DE_ASSERT(fs::exists(scripts[i].Path), "Failed to find script source file {}", scripts[i].Path);
            if (NeedToCompile(scripts[i].Path)) {
                recompile_ids.push_back(i);
            }
        }
        return recompile_ids;
    }
    std::optional<Path> ScriptManager::CompileSelected(const std::vector<ScriptAsset>& scripts, const std::vector<U32> ids) {
        for (auto id : ids) {
            if (!Compiler::Compile(scripts[id].Path, PathToCompiledScript(scripts[id].Path))) {
                return scripts[id].Path;
            }
            compiler_scripts_.insert(scripts[id].Path);
        }
        return {};
    }
    std::optional<std::string> ScriptManager::LinkLibrary(const std::vector<ScriptAsset>& scripts) {
        std::vector<Path> compiled_sources;
        for (const auto& script : scripts) {
            compiled_sources.push_back(PathToCompiledScript(script.Path));
        }
        std::string new_name = AvailableName();
        return (Compiler::Link(compiled_sources, Config::Get().ScriptCachePath, new_name) ? new_name : std::optional<std::string>());
    }
    bool ScriptManager::SwapLibrary(const std::string& name) {
        Ref<SharedObject> library = CreateRef<SharedObject>();
        if (!library->Load(Config::Get().ScriptCachePath, name)) {
            return false;
        }
        if (!library_name_.empty()) {
            ScriptEngine::DeleteLibrary(library_name_);
        }
        ScriptEngine::AddLibrary(library);
        library_name_ = name;
        return true;
    }
    bool ScriptManager::NeedToCompile(const Path& path) {
        if (!compiler_scripts_.contains(path)) {
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
        if (library_name_.empty()) {
            return "ScriptLibrary0";
        }
        return (library_name_.back() == '0' ? "ScriptLibrary1" : "ScriptLibrary0");
    }
}  // namespace DummyEngine