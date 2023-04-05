#include "DummyEditor/EditorLayer.h"

#include "DummyEditor/Scripting/Compiler.h"
#include "DummyEditor/Scripting/ScriptManager.h"

namespace DE {
    class MovingSystem : public System {
    public:
        MovingSystem() {}
        virtual std::string GetName() const override { return "MovingSystem"; }

        void Update(float dt) override {
            for (auto entity : View<RenderMeshComponent, TransformComponent>()) {
                entity.Get<RenderMeshComponent>().mesh_instance->at<Mat4>(0) = entity.Get<TransformComponent>().GetTransform();
            }

            for (auto entity : View<LightSource, TransformComponent>()) {
                entity.Get<LightSource>().position = entity.Get<TransformComponent>().translation;
            }
        }
    };

    EditorLayer::EditorLayer() : Layer("EditorLayer") {}

    void EditorLayer::OnAttach() {
        Compiler::Initialize();
        ScriptManager::Initialize();

        m_SceneData.m_FrameBuffer = FrameBuffer::Create({1920, 1080});
        m_SceneData.m_FrameBuffer->AddColorAttachment(TextureFormat::RGBA);
        m_SceneData.m_FrameBuffer->SetDepthAttachment(TextureFormat::DepthStencil);
        m_Viewport.SetFrameBuffer(m_SceneData.m_FrameBuffer);

        m_Panels.PushPanel(&m_Viewport);
        m_Panels.PushPanel(&m_SceneHierarchy);
        m_Panels.PushPanel(&m_Inspector);
        m_Panels.PushPanel(&m_Profiler);

        m_Viewport.SetController(m_State.m_ViewportEnabled);
        m_SceneHierarchy.SetController(m_State.m_SceneHierarchyEnabled);
        m_Inspector.SetController(m_State.m_InspectorEnabled);
        m_Profiler.SetController(m_State.m_ProfilerEnabled);
    }
    void EditorLayer::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("EditorLayer OnUpdate");

        ProcessControlls(dt);

        m_SceneData.m_FrameBuffer->Resize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
        m_SceneData.m_FrameBuffer->Bind();
        Renderer::OnWindowResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());

        if (m_SceneData.m_Scene) {
            m_SceneData.m_Scene->OnViewPortResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            m_SceneData.m_Scene->OnUpdate(dt);
            m_SceneData.m_Scene->Render();
        }

        m_SceneData.m_FrameBuffer->UnBind();
    }
    void EditorLayer::OnImGuiRender() {
        DE_PROFILE_SCOPE("EditorLayer OnImGuiRender");

        if (m_SceneData.m_Scene) {
            m_Inspector.SetActiveEntity(m_SceneHierarchy.GetActiveEntity());
        }

        ShowDockingSpace();

        m_Panels.OnImGuiRender();

        {
            DE_PROFILE_SCOPE("Demo Window");

            ImGui::ShowDemoWindow();
        }
    }

    void EditorLayer::OnDetach() {
        if (m_SceneData.m_Scene) {
            CloseScene();
        }

        ScriptManager::Terminate();
        Compiler::Terminate();
    }

    //*~~~EditorGUI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void EditorLayer::ShowDockingSpace() {
        DE_PROFILE_SCOPE("DockSpace");

        static bool p_open = true;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace",
                     &p_open,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar |
                         ImGuiWindowFlags_NoDocking);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        ShowDockingSpaceTabBar();

        ImGui::End();
    }
    void EditorLayer::ShowDockingSpaceTabBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Scene")) {
                    OpenSceneDialog();
                }
                if (ImGui::MenuItem("Save Scene")) {
                    SaveSceneDialog();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    WindowCloseEvent event;
                    BroadcastEvent(event);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Viewport", NULL, &m_State.m_ViewportEnabled);
                ImGui::MenuItem("Scene Hierarchy", NULL, &m_State.m_SceneHierarchyEnabled);
                ImGui::MenuItem("Inspector", NULL, &m_State.m_InspectorEnabled);
                ImGui::MenuItem("Profiler", NULL, &m_State.m_ProfilerEnabled);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    //*~~~EditorFunctionality~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void EditorLayer::OpenSceneDialog() {
        Path path = FileSystem::OpenFileDialog("Dummy Engine Scene (*.yml)", "*.yml");
        if (path != Path()) {
            if (m_SceneData.m_Scene) {
                CloseScene();
            }
            OpenScene(path);
        }
    }
    void EditorLayer::SaveSceneDialog() {
        if (m_SceneData.m_Scene) {
            Path path = FileSystem::SaveFileDialog("Dummy Engine Scene (*.yml)", "*.yml");
            if (path != Path()) {
                SaveScene(path);
            }
        }
    }
    void EditorLayer::OpenScene(const Path& scene_path) {
        auto data            = SceneLoader::LoadSerializationData(scene_path);
        m_SceneData.m_Assets = std::move(data.assets);
        LoadAssets();
        ScriptManager::LoadScripts(m_SceneData.m_Assets.scripts);

        m_SceneData.m_Scene = SceneLoader::Instantiate(data);

        PrepareScene();
        ScriptManager::AttachScripts(m_SceneData.m_Scene);
        LOG_INFO("EditorLayer", "Opened scene");
    }
    void EditorLayer::SaveScene(const Path& path) {
        SceneLoader::Save(m_SceneData.m_Scene, m_SceneData.m_Assets, path);
    }
    void EditorLayer::ReloadScripts() {
        ScriptManager::ReloadScripts(m_SceneData.m_Assets.scripts, m_SceneData.m_Scene);
    }
    void EditorLayer::CloseScene() {
        m_Inspector.SetActiveEntity(Entity());
        m_SceneHierarchy.UnSelect();
        m_SceneHierarchy.SetActiveScene(nullptr);
        m_SceneData.m_Scene = nullptr;
        ResourceManager::Clear();
        ScriptManager::UnloadScripts(m_SceneData.m_Assets.scripts);
        UnloadAssets();
        LOG_INFO("EditorLayer", "Closed scene");
    }

    void EditorLayer::LoadAssets() {
        for (const auto& asset : m_SceneData.m_Assets.textures) {
            AssetManager::AddTextureAsset(asset);
        }
        for (const auto& asset : m_SceneData.m_Assets.scripts) {
            AssetManager::AddScriptAsset(asset);
        }
        for (const auto& asset : m_SceneData.m_Assets.render_meshes) {
            AssetManager::AddRenderMeshAsset(asset);
        }
        for (const auto& asset : m_SceneData.m_Assets.shaders) {
            AssetManager::AddShaderAsset(asset);
        }
    }
    void EditorLayer::UnloadAssets() {
        for (const auto& asset : m_SceneData.m_Assets.textures) {
            AssetManager::RemoveTextureAsset(asset.id);
        }
        for (const auto& asset : m_SceneData.m_Assets.scripts) {
            AssetManager::RemoveScriptAsset(asset.id);
        }
        for (const auto& asset : m_SceneData.m_Assets.render_meshes) {
            AssetManager::RemoveRenderMeshAsset(asset.id);
        }
        for (const auto& asset : m_SceneData.m_Assets.shaders) {
            AssetManager::RemoveShaderAsset(asset.id);
        }
    }
    void EditorLayer::PrepareScene() {
        m_EditorCamera = m_SceneData.m_Scene->CreateHiddenEntity("Editor Camera");
        m_EditorCamera.AddComponent<TransformComponent>();
        m_EditorCamera.AddComponent<FPSCamera>();
        m_EditorCamera.AddComponent<ScriptComponent>(ScriptEngine::CreateScript(ScriptManager::EditorScript("EditorCameraController")));

        m_SceneData.m_Scene->RegisterSystem<MovingSystem>();
        m_SceneHierarchy.SetActiveScene(m_SceneData.m_Scene);
        m_Inspector.SetScene(m_SceneData.m_Scene);
        m_SceneHierarchy.UnSelect();
        m_Inspector.SetActiveEntity(m_SceneHierarchy.GetActiveEntity());
    }

    void EditorLayer::ProcessControlls(float dt) {
        DE_PROFILE_SCOPE("ProcessControlls");

        if (Input::KeyDown(Key::LeftControl)) {
            if (Input::KeyReleased(Key::GraveAccent) && m_SceneData.m_Scene != nullptr) {
                m_State.m_InputState = (m_State.m_InputState == InputState::ViewPort ? InputState::NonSpecified : InputState::ViewPort);
                SetMouseLockToggleEvent event;
                BroadcastEvent(event);
            }
            if (m_State.m_InputState != InputState::ViewPort) {
                if (Input::KeyReleased(Key::O)) {
                    OpenSceneDialog();
                }
                if (Input::KeyReleased(Key::S)) {
                    SaveSceneDialog();
                }
                if (Input::KeyReleased(Key::X)) {
                    if (m_SceneData.m_Scene) {
                        CloseScene();
                    }
                }
                if (Input::KeyReleased(Key::R)) {
                    if (m_SceneData.m_Scene) {
                        ReloadScripts();
                    }
                }
            }
        }
        if (m_EditorCamera.Valid()) {
            bool& active = m_EditorCamera.Get<ScriptComponent>()->GetField<bool>("active");
            if (m_State.m_InputState == InputState::ViewPort) {
                active = true;
            } else {
                active = false;
            }
        }
    }
}  // namespace DE