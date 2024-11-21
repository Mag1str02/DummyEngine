#include "DummyEditor/EditorLayer.h"

#include "DummyEditor/Scripting/Compiler.h"
#include "DummyEditor/Scripting/ScriptManager.h"
#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Application/FileSystem.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DummyEngine {

    //*___Layer________________________________________________________________________________________________________________________________________________________________________________________

    void EditorLayer::OnAttach() {
        Compiler::Initialize();
        ScriptManager::Initialize();

        imgui_manager_.LoadEditorResources();
        imgui_manager_.AddPanel(&viewport_);
        imgui_manager_.AddPanel(&shadow_viewport_);
        imgui_manager_.AddPanel(&scene_hierarchy_);
        imgui_manager_.AddPanel(&inspector_);
        imgui_manager_.AddPanel(&profiler_);
        imgui_manager_.AddPanel(&theme_panel_);
        imgui_manager_.AddPanel(&renderer_panel_);
        imgui_manager_.SetMenuBar(&menu_bar_);

        ts_system_ = CreateRef<TransformSyncSystem>();
        LoadIcons();
    }
    void EditorLayer::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("EditorLayer OnUpdate");

        ProcessControlls(dt);

        if (Console::GetBool("r_shadowmap_fbo")) {
            shadow_viewport_.SetFrameBuffer(current_scene_->GetRenderer()->GetShadowMap());
            shadow_viewport_.UseDepthAttachment(true);
        }

        switch (scene_state_) {
            case SceneState::Running: {
                Entity camera = (current_scene_->HasCamera() ? Entity() : editor_camera_);
                current_scene_->OnUpdate(dt);
                current_scene_->OnViewPortResize(viewport_.GetWidth(), viewport_.GetHeight());
                current_scene_->OnRender(camera);
                break;
            }
            case SceneState::Step:
                current_scene_->OnUpdate(dt);
                current_scene_->OnViewPortResize(viewport_.GetWidth(), viewport_.GetHeight());
                current_scene_->OnRender(editor_camera_);
                scene_state_ = SceneState::Paused;
                break;
            case SceneState::Paused:
            case SceneState::Editing:
                ts_system_->Update(dt);
                editor_camera_.Get<ScriptComponent>()->OnUpdate(dt);
                current_scene_->OnViewPortResize(viewport_.GetWidth(), viewport_.GetHeight());
                current_scene_->OnRender(editor_camera_);
                break;
            case SceneState::None: break;
            default: DE_ASSERT(false, "Unsupported SceneState"); break;
        }
    }
    void EditorLayer::OnImGuiRender() {
        DE_PROFILE_SCOPE("EditorLayer OnImGuiRender");

        if (current_scene_) {
            inspector_.SetActiveEntity(scene_hierarchy_.GetActiveEntity());
        }

        imgui_manager_.OnImGui();
        // ImGui::ShowDemoWindow();
        // ImGui::Begin("Cal");
        // ImGui::ShowStyleEditor();
        // ImGui::End();
        // ImGui::ShowStackToolWindow();
    }
    void EditorLayer::OnDetach() {
        if (current_scene_) {
            CloseScene();
        }

        ScriptManager::Terminate();
        Compiler::Terminate();
    }

    //*___EditorActions________________________________________________________________________________________________________________________________________________________________________________

    void EditorLayer::ActionOpenScene() {
        Path p = OpenSceneDialog();
        if (!p.empty()) {
            if (current_scene_) {
                CloseScene();
            }
            OpenScene(p);
        }
    }
    void EditorLayer::ActionCloseScene() {
        if (current_scene_) {
            CloseScene();
        }
    }
    void EditorLayer::ActionSaveScene() {
        if (current_scene_) {
            Path p = SaveSceneDialog();
            if (!p.empty()) {
                SaveScene(p);
            }
        }
    }
    void EditorLayer::ActionCreateScene() {
        if (current_scene_) {
            CloseScene();
        }
        OpenScene(Path());
    }
    void EditorLayer::ActionBuild() {
        if (current_scene_) {
            ReloadScripts();
        }
    }
    void EditorLayer::ActionBuildAndRun() {
        if (current_scene_) {
            ReloadScripts();
            if (scene_script_state_ == SceneScriptState::Compiled) {
                RunScene();
            }
        }
    }
    void EditorLayer::ActionRunScene() {
        if (current_scene_ && scene_script_state_ != SceneScriptState::Uncompiled) {
            RunScene();
        }
    }
    void EditorLayer::ActionStopScene() {
        if (current_scene_) {
            StopScene();
        }
    }
    void EditorLayer::ActionPauseScene() {
        if (current_scene_ && scene_state_ == SceneState::Running) {
            scene_state_ = SceneState::Paused;
            current_scene_->OnRuntimePause();
        }
    }
    void EditorLayer::ActionResumeScene() {
        if (current_scene_ && scene_state_ == SceneState::Paused) {
            scene_state_ = SceneState::Running;
            current_scene_->OnRuntimeResume();
        }
    }
    void EditorLayer::ActionStepScene() {
        if (current_scene_ && scene_state_ == SceneState::Paused) {
            scene_state_ = SceneState::Step;
        }
    }

    //*___Editor_______________________________________________________________________________________________________________________________________________________________________________________

    EditorLayer::EditorLayer() : Layer("EditorLayer"), shadow_viewport_("fbo_viewport") {
        DE_ASSERT(gInstance == nullptr, "Editor layer already created");
        gInstance = this;
    }
    EditorLayer::~EditorLayer() {
        gInstance = nullptr;
    }
    EditorLayer& EditorLayer::Get() {
        return *gInstance;
    }
    ImGuiManager& EditorLayer::GetImGuiManager() {
        return imgui_manager_;
    }
    EditorLayer::SceneScriptState EditorLayer::GetSceneScriptState() const {
        return scene_script_state_;
    }
    EditorLayer::SceneState EditorLayer::GetSceneState() const {
        return scene_state_;
    }
    EditorLayer::InputState EditorLayer::GetInputState() const {
        return input_state_;
    }
    EditorLayer::Resources& EditorLayer::GetResources() {
        return resources_;
    }
    Ref<Scene> EditorLayer::GetScene() const {
        return current_scene_;
    }

    //*___Helpers______________________________________________________________________________________________________________________________________________________________________________________

    void EditorLayer::ReloadScripts() {
        auto res = ScriptManager::ReloadScripts(scene_file_data_.Assets.Scripts, current_scene_);
        if (scene_script_state_ == SceneScriptState::Uncompiled) {
            if (res) {
                scene_script_state_ = SceneScriptState::Compiled;
            }
        }
        if (scene_script_state_ == SceneScriptState::Compiled || scene_script_state_ == SceneScriptState::OldCompiled) {
            if (res) {
                scene_script_state_ = SceneScriptState::Compiled;
            } else {
                scene_script_state_ = SceneScriptState::OldCompiled;
            }
        }
    }

    Path EditorLayer::OpenSceneDialog() {
        return FileSystem::OpenFileDialog("Dummy Engine Scene (*.yml)", "yml", Config::Get().ScenePath);
    }
    Path EditorLayer::SaveSceneDialog() {
        return FileSystem::SaveFileDialog("Dummy Engine Scene (*.yml)", "yml", "", Config::Get().ScenePath);
    }
    void EditorLayer::OpenScene(const Path& scene_path) {
        if (scene_path.empty()) {
            scene_file_data_ = SceneFileData();
            current_scene_   = CreateRef<Scene>();
        } else {
            auto res = SceneLoader::LoadScene(scene_path);
            if (!res) {
                return;
            }
            scene_file_data_ = res.value();
            LoadAssets();
            if (!ScriptManager::LoadScripts(scene_file_data_.Assets.Scripts)) {
                return;
            }
            current_scene_ = SceneLoader::Serialize(scene_file_data_.Hierarchy);
            if (current_scene_ == nullptr) {
                return;
            }
            current_scene_->GetRenderer()->Settings = scene_file_data_.Settings;
        }
        PrepareScene();
        ScriptManager::AttachScripts(current_scene_);
        viewport_.SetFrameBuffer(current_scene_->GetRenderer()->GetFrameBuffer());
        scene_state_        = SceneState::Editing;
        scene_script_state_ = SceneScriptState::Compiled;
        current_scene_->LoadPhysics(current_scene_);
        LOG_INFO("Opened scene");
    }
    void EditorLayer::SaveScene(const Path& path) {
        scene_file_data_.Hierarchy = SceneLoader::Deserialize(current_scene_);
        scene_file_data_.Settings  = current_scene_->GetRenderer()->Settings;
        SceneLoader::SaveScene(scene_file_data_, path);
    }
    void EditorLayer::CloseScene() {
        inspector_.SetActiveEntity(Entity());
        current_scene_      = nullptr;
        scene_state_        = SceneState::None;
        scene_script_state_ = SceneScriptState::Uncompiled;
        ResourceManager::Clear();
        ScriptManager::UnloadScripts(scene_file_data_.Assets.Scripts);
        UnloadAssets();
        LOG_INFO("Closed scene");
    }
    void EditorLayer::RunScene() {
        current_scene_->OnRuntimeStart();
        scene_state_ = SceneState::Running;
    }
    void EditorLayer::StopScene() {
        current_scene_->OnRuntimeStop();
        scene_state_ = SceneState::Editing;
    }

    void EditorLayer::LoadAssets() {
        for (const auto& asset : scene_file_data_.Assets.Textures) {
            AssetManager::AddTextureAsset(asset);
        }
        for (const auto& asset : scene_file_data_.Assets.Scripts) {
            AssetManager::AddScriptAsset(asset);
        }
        for (const auto& asset : scene_file_data_.Assets.RenderMeshes) {
            AssetManager::AddRenderMeshAsset(asset);
        }
        for (const auto& asset : scene_file_data_.Assets.Shaders) {
            AssetManager::AddShaderAsset(asset);
        }
    }
    void EditorLayer::UnloadAssets() {
        for (const auto& asset : scene_file_data_.Assets.Textures) {
            AssetManager::RemoveTextureAsset(asset.ID);
        }
        for (const auto& asset : scene_file_data_.Assets.Scripts) {
            AssetManager::RemoveScriptAsset(asset.ID);
        }
        for (const auto& asset : scene_file_data_.Assets.RenderMeshes) {
            AssetManager::RemoveRenderMeshAsset(asset.ID);
        }
        for (const auto& asset : scene_file_data_.Assets.Shaders) {
            AssetManager::RemoveShaderAsset(asset.ID);
        }
    }
    void EditorLayer::PrepareScene() {
        editor_camera_ = current_scene_->CreateEntity("Editor Camera", false);
        editor_camera_.AddComponent<TransformComponent>();
        editor_camera_.AddComponent<FPSCamera>();
        editor_camera_.AddComponent<ScriptComponent>(ScriptEngine::CreateScript(ScriptManager::EditorScript("EditorCameraController")));

        current_scene_->AttachSystem(ts_system_);
        scene_hierarchy_.SetActiveScene(current_scene_);
        inspector_.SetScene(current_scene_);
        renderer_panel_.SetScene(current_scene_);
        inspector_.SetActiveEntity(scene_hierarchy_.GetActiveEntity());
    }

    void EditorLayer::ProcessControlls(float) {
        DE_PROFILE_SCOPE("ProcessControlls");

        if (Input::KeyDown(Key::LeftControl) && Input::KeyDown(Key::LeftShift)) {
            if (Input::KeyReleased(Key::N)) {
                ActionCreateScene();
            }
        }
        if (Input::KeyDown(Key::LeftControl)) {
            if (Input::KeyReleased(Key::GraveAccent) && current_scene_ != nullptr) {
                input_state_ = (input_state_ == InputState::ViewPort ? InputState::NonSpecified : InputState::ViewPort);
                SetMouseLockToggleEvent event;
                BroadcastEvent(event);
            }
            if (input_state_ != InputState::ViewPort) {
                if (Input::KeyReleased(Key::O)) {
                    ActionOpenScene();
                }
                if (Input::KeyReleased(Key::S)) {
                    ActionSaveScene();
                }
                if (Input::KeyReleased(Key::X)) {
                    ActionCloseScene();
                }
                if (Input::KeyReleased(Key::R)) {
                    ActionBuild();
                }
            }
        }
        if (editor_camera_.Valid()) {
            bool& active = editor_camera_.Get<ScriptComponent>()->GetField<bool>("active_");
            if (input_state_ == InputState::ViewPort) {
                active = true;
            } else {
                active = false;
            }
        }
    }

    void EditorLayer::LoadEditorResources() {}
    void EditorLayer::LoadIcons() {
        auto play_data          = TextureLoader::Load({Config::Get().ExecutablePath / "Editor/Icons/PlayButton.png"});
        auto pause_data         = TextureLoader::Load({Config::Get().ExecutablePath / "Editor/Icons/PauseButton.png"});
        auto step_data          = TextureLoader::Load({Config::Get().ExecutablePath / "Editor/Icons/StepButton.png"});
        auto stop_data          = TextureLoader::Load({Config::Get().ExecutablePath / "Editor/Icons/StopButton.png"});
        auto build_data         = TextureLoader::Load({Config::Get().ExecutablePath / "Editor/Icons/BuildButton.png"});
        auto build_and_run_data = TextureLoader::Load({Config::Get().ExecutablePath / "Editor/Icons/BuildAndRunButton.png"});

        DE_ASSERT(play_data, "Failed to load play icon");
        DE_ASSERT(pause_data, "Failed to load pause icon");
        DE_ASSERT(step_data, "Failed to load step icon");
        DE_ASSERT(stop_data, "Failed to load stop icon");
        DE_ASSERT(build_data, "Failed to load build icon");
        DE_ASSERT(build_and_run_data, "Failed to load build and run icon");

        resources_.PlayIcon        = Texture::Create({*play_data});
        resources_.PauseIcon       = Texture::Create({*pause_data});
        resources_.StepIcon        = Texture::Create({*step_data});
        resources_.StopIcon        = Texture::Create({*stop_data});
        resources_.BuildIcon       = Texture::Create({*build_data});
        resources_.BuildAndRunIcon = Texture::Create({*build_and_run_data});
    }

    //*___Other________________________________________________________________________________________________________________________________________________________________________________________

    void TransformSyncSystem::Update(float) {
        for (auto entity : View<LightSource, TransformComponent>()) {
            entity.Get<LightSource>().Position = entity.Get<TransformComponent>().Translation;
        }
    }

    EditorLayer* EditorLayer::gInstance = nullptr;

}  // namespace DummyEngine