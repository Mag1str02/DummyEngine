#include "DummyEditor/EditorLayer.h"

#include "DummyEditor/Scripting/Compiler.h"
#include "DummyEditor/Scripting/ScriptManager.h"

namespace DE {

    //*___Layer________________________________________________________________________________________________________________________________________________________________________________________

    void EditorLayer::OnAttach() {
        Compiler::Initialize();
        ScriptManager::Initialize();

        m_ImGuiManager.LoadEditorResources();
        m_ImGuiManager.AddPanel(&m_Viewport);
        m_ImGuiManager.AddPanel(&m_SceneHierarchy);
        m_ImGuiManager.AddPanel(&m_Inspector);
        m_ImGuiManager.AddPanel(&m_Profiler);
        m_ImGuiManager.AddPanel(&m_ThemePanel);
        m_ImGuiManager.AddPanel(&m_RendererPanel);
        m_ImGuiManager.SetMenuBar(&m_MenuBar);

        m_TSSystem = CreateRef<TransformSyncSystem>();
        LoadIcons();
    }
    void EditorLayer::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("EditorLayer OnUpdate");

        ProcessControlls(dt);

        switch (m_SceneState) {
            case SceneState::Running: {
                Entity camera = (m_CurrentScene->HasCamera() ? Entity() : m_EditorCamera);
                m_CurrentScene->OnUpdate(dt);
                m_CurrentScene->OnViewPortResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
                m_CurrentScene->OnRender(camera);
                break;
            }
            case SceneState::Step:
                m_CurrentScene->OnUpdate(dt);
                m_CurrentScene->OnViewPortResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
                m_CurrentScene->OnRender(m_EditorCamera);
                m_SceneState = SceneState::Paused;
                break;
            case SceneState::Paused:
            case SceneState::Editing:
                m_TSSystem->Update(dt);
                m_EditorCamera.Get<ScriptComponent>()->OnUpdate(dt);
                m_CurrentScene->OnViewPortResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
                m_CurrentScene->OnRender(m_EditorCamera);
                break;
            case SceneState::None: break;
            default: DE_ASSERT(false, "Unsupported SceneState"); break;
        }
    }
    void EditorLayer::OnImGuiRender() {
        DE_PROFILE_SCOPE("EditorLayer OnImGuiRender");

        if (m_CurrentScene) {
            m_Inspector.SetActiveEntity(m_SceneHierarchy.GetActiveEntity());
        }

        m_ImGuiManager.OnImGui();
        // ImGui::ShowDemoWindow();
        // ImGui::Begin("Cal");
        // ImGui::ShowStyleEditor();
        // ImGui::End();
        // ImGui::ShowStackToolWindow();
    }
    void EditorLayer::OnDetach() {
        if (m_CurrentScene) {
            CloseScene();
        }

        ScriptManager::Terminate();
        Compiler::Terminate();
    }

    //*___EditorActions________________________________________________________________________________________________________________________________________________________________________________

    void EditorLayer::ActionOpenScene() {
        Path p = OpenSceneDialog();
        if (!p.empty()) {
            if (m_CurrentScene) {
                CloseScene();
            }
            OpenScene(p);
        }
    }
    void EditorLayer::ActionCloseScene() {
        if (m_CurrentScene) {
            CloseScene();
        }
    }
    void EditorLayer::ActionSaveScene() {
        if (m_CurrentScene) {
            Path p = SaveSceneDialog();
            if (!p.empty()) {
                SaveScene(p);
            }
        }
    }
    void EditorLayer::ActionCreateScene() {
        if (m_CurrentScene) {
            CloseScene();
        }
        OpenScene(Path());
    }
    void EditorLayer::ActionBuild() {
        if (m_CurrentScene) {
            ReloadScripts();
        }
    }
    void EditorLayer::ActionBuildAndRun() {
        if (m_CurrentScene) {
            ReloadScripts();
            if (m_SceneScriptState == SceneScriptState::Compiled) {
                RunScene();
            }
        }
    }
    void EditorLayer::ActionRunScene() {
        if (m_CurrentScene && m_SceneScriptState != SceneScriptState::Uncompiled) {
            RunScene();
        }
    }
    void EditorLayer::ActionStopScene() {
        if (m_CurrentScene) {
            StopScene();
        }
    }
    void EditorLayer::ActionPauseScene() {
        if (m_CurrentScene && m_SceneState == SceneState::Running) {
            m_SceneState = SceneState::Paused;
            m_CurrentScene->OnRuntimePause();
        }
    }
    void EditorLayer::ActionResumeScene() {
        if (m_CurrentScene && m_SceneState == SceneState::Paused) {
            m_SceneState = SceneState::Running;
            m_CurrentScene->OnRuntimeResume();
        }
    }
    void EditorLayer::ActionStepScene() {
        if (m_CurrentScene && m_SceneState == SceneState::Paused) {
            m_SceneState = SceneState::Step;
        }
    }

    //*___Editor_______________________________________________________________________________________________________________________________________________________________________________________

    EditorLayer::EditorLayer() : Layer("EditorLayer") {
        DE_ASSERT(s_Instance == nullptr, "Editor layer already created");
        s_Instance = this;
    }
    EditorLayer::~EditorLayer() {
        s_Instance = nullptr;
    }
    EditorLayer& EditorLayer::Get() {
        return *s_Instance;
    }
    ImGuiManager& EditorLayer::GetImGuiManager() {
        return m_ImGuiManager;
    }
    EditorLayer::SceneScriptState EditorLayer::GetSceneScriptState() const {
        return m_SceneScriptState;
    }
    EditorLayer::SceneState EditorLayer::GetSceneState() const {
        return m_SceneState;
    }
    EditorLayer::InputState EditorLayer::GetInputState() const {
        return m_InputState;
    }
    EditorLayer::Resources& EditorLayer::GetResources() {
        return m_Resources;
    }
    Ref<Scene> EditorLayer::GetScene() const {
        return m_CurrentScene;
    }

    //*___Helpers______________________________________________________________________________________________________________________________________________________________________________________

    void EditorLayer::ReloadScripts() {
        auto res = ScriptManager::ReloadScripts(m_SceneFileData.assets.scripts, m_CurrentScene);
        if (m_SceneScriptState == SceneScriptState::Uncompiled) {
            if (res) {
                m_SceneScriptState = SceneScriptState::Compiled;
            }
        }
        if (m_SceneScriptState == SceneScriptState::Compiled || m_SceneScriptState == SceneScriptState::OldCompiled) {
            if (res) {
                m_SceneScriptState = SceneScriptState::Compiled;
            } else {
                m_SceneScriptState = SceneScriptState::OldCompiled;
            }
        }
    }

    Path EditorLayer::OpenSceneDialog() {
        return FileSystem::OpenFileDialog("Dummy Engine Scene (*.yml)", "yml", Config::GetPath(DE_CFG_SCENE_PATH));
    }
    Path EditorLayer::SaveSceneDialog() {
        return FileSystem::SaveFileDialog("Dummy Engine Scene (*.yml)", "yml", "", Config::GetPath(DE_CFG_SCENE_PATH));
    }
    void EditorLayer::OpenScene(const Path& scene_path) {
        if (scene_path.empty()) {
            m_SceneFileData = SceneFileData();
            m_CurrentScene  = CreateRef<Scene>();
        } else {
            auto res = SceneLoader::LoadScene(scene_path);
            if (!res) {
                return;
            }
            m_SceneFileData = res.value();
            LoadAssets();
            if (!ScriptManager::LoadScripts(m_SceneFileData.assets.scripts)) {
                return;
            }
            m_CurrentScene                          = SceneLoader::Serialize(m_SceneFileData.hierarchy);
            m_CurrentScene->GetRenderer()->settings = m_SceneFileData.settings;
        }
        PrepareScene();
        ScriptManager::AttachScripts(m_CurrentScene);
        m_Viewport.SetFrameBuffer(m_CurrentScene->GetRenderer()->GetFrameBuffer());
        m_SceneState       = SceneState::Editing;
        m_SceneScriptState = SceneScriptState::Compiled;
        m_CurrentScene->LoadPhysics(m_CurrentScene);
        LOG_INFO("EditorLayer", "Opened scene");
    }
    void EditorLayer::SaveScene(const Path& path) {
        m_SceneFileData.hierarchy = SceneLoader::Deserialize(m_CurrentScene);
        m_SceneFileData.settings  = m_CurrentScene->GetRenderer()->settings;
        SceneLoader::SaveScene(m_SceneFileData, path);
    }
    void EditorLayer::CloseScene() {
        m_Inspector.SetActiveEntity(Entity());
        m_CurrentScene     = nullptr;
        m_SceneState       = SceneState::None;
        m_SceneScriptState = SceneScriptState::Uncompiled;
        ResourceManager::Clear();
        ScriptManager::UnloadScripts(m_SceneFileData.assets.scripts);
        UnloadAssets();
        LOG_INFO("EditorLayer", "Closed scene");
    }
    void EditorLayer::RunScene() {
        m_CurrentScene->OnRuntimeStart();
        m_SceneState = SceneState::Running;
    }
    void EditorLayer::StopScene() {
        m_CurrentScene->OnRuntimeStop();
        m_SceneState = SceneState::Editing;
    }

    void EditorLayer::LoadAssets() {
        for (const auto& asset : m_SceneFileData.assets.textures) {
            AssetManager::AddTextureAsset(asset);
        }
        for (const auto& asset : m_SceneFileData.assets.scripts) {
            AssetManager::AddScriptAsset(asset);
        }
        for (const auto& asset : m_SceneFileData.assets.render_meshes) {
            AssetManager::AddRenderMeshAsset(asset);
        }
        for (const auto& asset : m_SceneFileData.assets.shaders) {
            AssetManager::AddShaderAsset(asset);
        }
    }
    void EditorLayer::UnloadAssets() {
        for (const auto& asset : m_SceneFileData.assets.textures) {
            AssetManager::RemoveTextureAsset(asset.id);
        }
        for (const auto& asset : m_SceneFileData.assets.scripts) {
            AssetManager::RemoveScriptAsset(asset.id);
        }
        for (const auto& asset : m_SceneFileData.assets.render_meshes) {
            AssetManager::RemoveRenderMeshAsset(asset.id);
        }
        for (const auto& asset : m_SceneFileData.assets.shaders) {
            AssetManager::RemoveShaderAsset(asset.id);
        }
    }
    void EditorLayer::PrepareScene() {
        m_EditorCamera = m_CurrentScene->CreateEntity("Editor Camera", false);
        m_EditorCamera.AddComponent<TransformComponent>();
        m_EditorCamera.AddComponent<FPSCamera>();
        m_EditorCamera.AddComponent<ScriptComponent>(ScriptEngine::CreateScript(ScriptManager::EditorScript("EditorCameraController")));

        m_CurrentScene->AttachSystem(m_TSSystem);
        m_SceneHierarchy.SetActiveScene(m_CurrentScene);
        m_Inspector.SetScene(m_CurrentScene);
        m_RendererPanel.SetScene(m_CurrentScene);
        m_Inspector.SetActiveEntity(m_SceneHierarchy.GetActiveEntity());
    }

    void EditorLayer::ProcessControlls(float dt) {
        DE_PROFILE_SCOPE("ProcessControlls");

        if (Input::KeyDown(Key::LeftControl) && Input::KeyDown(Key::LeftShift)) {
            if (Input::KeyReleased(Key::N)) {
                ActionCreateScene();
            }
        }
        if (Input::KeyDown(Key::LeftControl)) {
            if (Input::KeyReleased(Key::GraveAccent) && m_CurrentScene != nullptr) {
                m_InputState = (m_InputState == InputState::ViewPort ? InputState::NonSpecified : InputState::ViewPort);
                SetMouseLockToggleEvent event;
                BroadcastEvent(event);
            }
            if (m_InputState != InputState::ViewPort) {
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
        if (m_EditorCamera.Valid()) {
            bool& active = m_EditorCamera.Get<ScriptComponent>()->GetField<bool>("active");
            if (m_InputState == InputState::ViewPort) {
                active = true;
            } else {
                active = false;
            }
        }
    }

    void EditorLayer::LoadEditorResources() {}
    void EditorLayer::LoadIcons() {
        auto play_data          = TextureLoader::Load({Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "Editor/Icons/PlayButton.png"});
        auto pause_data         = TextureLoader::Load({Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "Editor/Icons/PauseButton.png"});
        auto step_data          = TextureLoader::Load({Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "Editor/Icons/StepButton.png"});
        auto stop_data          = TextureLoader::Load({Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "Editor/Icons/StopButton.png"});
        auto build_data         = TextureLoader::Load({Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "Editor/Icons/BuildButton.png"});
        auto build_and_run_data = TextureLoader::Load({Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "Editor/Icons/BuildAndRunButton.png"});

        DE_ASSERT(play_data, "Failed to load play icon");
        DE_ASSERT(pause_data, "Failed to load pause icon");
        DE_ASSERT(step_data, "Failed to load step icon");
        DE_ASSERT(stop_data, "Failed to load stop icon");
        DE_ASSERT(build_data, "Failed to load build icon");
        DE_ASSERT(build_and_run_data, "Failed to load build and run icon");

        m_Resources.play_icon          = Texture::Create({*play_data});
        m_Resources.pause_icon         = Texture::Create({*pause_data});
        m_Resources.step_icon          = Texture::Create({*step_data});
        m_Resources.stop_icon          = Texture::Create({*stop_data});
        m_Resources.build_icon         = Texture::Create({*build_data});
        m_Resources.build_and_run_icon = Texture::Create({*build_and_run_data});
    }

    //*___Other________________________________________________________________________________________________________________________________________________________________________________________

    void TransformSyncSystem::Update(float dt) {
        for (auto entity : View<LightSource, TransformComponent>()) {
            entity.Get<LightSource>().position = entity.Get<TransformComponent>().translation;
        }
    }

    EditorLayer* EditorLayer::s_Instance = nullptr;

}  // namespace DE