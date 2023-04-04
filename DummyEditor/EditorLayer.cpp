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

        m_SceneData.frame_buffer = FrameBuffer::Create({1920, 1080});
        m_SceneData.frame_buffer->AddColorAttachment(TextureFormat::RGBA);
        m_SceneData.frame_buffer->SetDepthAttachment(TextureFormat::DepthStencil);
        m_Viewport.SetFrameBuffer(m_SceneData.frame_buffer);

        m_ImGuiManager.LoadEditorResources();
        m_ImGuiManager.AddPanel(&m_Viewport);
        m_ImGuiManager.AddPanel(&m_SceneHierarchy);
        m_ImGuiManager.AddPanel(&m_Inspector);
        m_ImGuiManager.AddPanel(&m_Profiler);
        m_ImGuiManager.AddPanel(&m_ThemePanel);
        m_ImGuiManager.SetMenuBar(&m_MenuBar);
        m_MenuBar.AttachToEditor(this);
    }
    void EditorLayer::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("EditorLayer OnUpdate");

        ProcessControlls(dt);

        m_SceneData.frame_buffer->Resize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
        m_SceneData.frame_buffer->Bind();
        Renderer::OnWindowResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());

        if (m_SceneData.scene) {
            m_SceneData.scene->OnViewPortResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            m_SceneData.scene->OnUpdate(dt);
            m_SceneData.scene->Render();
        }

        m_SceneData.frame_buffer->UnBind();
    }
    void EditorLayer::OnImGuiRender() {
        DE_PROFILE_SCOPE("EditorLayer OnImGuiRender");

        if (m_SceneData.scene) {
            m_Inspector.SetActiveEntity(m_SceneHierarchy.GetActiveEntity());
        }

        m_ImGuiManager.OnImGui();

        ImGui::Begin("Dear ImGui Style Editor");
        ImGui::ShowStyleEditor();
        ImGui::End();
        ImGui::ShowDemoWindow();
    }

    void EditorLayer::OnDetach() {
        if (m_SceneData.scene) {
            CloseScene();
        }

        ScriptManager::Terminate();
        Compiler::Terminate();
    }

    //*~~~EditorFunctionality~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void EditorLayer::OpenSceneDialog() {
        Path path = FileSystem::OpenFileDialog("Dummy Engine Scene (*.yml)", "*.yml");
        if (path != Path()) {
            if (m_SceneData.scene) {
                CloseScene();
            }
            OpenScene(path);
        }
    }
    void EditorLayer::SaveSceneDialog() {
        if (m_SceneData.scene) {
            Path path = FileSystem::SaveFileDialog("Dummy Engine Scene (*.yml)", "*.yml");
            if (path != Path()) {
                SaveScene(path);
            }
        }
    }
    void EditorLayer::OpenScene(const Path& scene_path) {
        auto res = SceneLoader::LoadScene(scene_path);
        if (!res) {
            return;
        }
        m_SceneData.file_data = res.value();
        LoadAssets();
        ScriptManager::LoadScripts(m_SceneData.file_data.assets.scripts);

        m_SceneData.scene = SceneLoader::Serialize(m_SceneData.file_data.hierarchy);

        PrepareScene();
        ScriptManager::AttachScripts(m_SceneData.scene);
        LOG_INFO("EditorLayer", "Opened scene");
    }
    void EditorLayer::SaveScene(const Path& path) {
        m_SceneData.file_data.hierarchy = SceneLoader::Deserialize(m_SceneData.scene);
        SceneLoader::SaveScene(m_SceneData.file_data, path);
    }
    void EditorLayer::ReloadScripts() {
        ScriptManager::ReloadScripts(m_SceneData.file_data.assets.scripts, m_SceneData.scene);
    }
    void EditorLayer::CloseScene() {
        m_Inspector.SetActiveEntity(Entity());
        m_SceneData.scene = nullptr;
        ResourceManager::Clear();
        ScriptManager::UnloadScripts(m_SceneData.file_data.assets.scripts);
        UnloadAssets();
        LOG_INFO("EditorLayer", "Closed scene");
    }

    void EditorLayer::LoadAssets() {
        for (const auto& asset : m_SceneData.file_data.assets.textures) {
            AssetManager::AddTextureAsset(asset);
        }
        for (const auto& asset : m_SceneData.file_data.assets.scripts) {
            AssetManager::AddScriptAsset(asset);
        }
        for (const auto& asset : m_SceneData.file_data.assets.render_meshes) {
            AssetManager::AddRenderMeshAsset(asset);
        }
        for (const auto& asset : m_SceneData.file_data.assets.shaders) {
            AssetManager::AddShaderAsset(asset);
        }
    }
    void EditorLayer::UnloadAssets() {
        for (const auto& asset : m_SceneData.file_data.assets.textures) {
            AssetManager::RemoveTextureAsset(asset.id);
        }
        for (const auto& asset : m_SceneData.file_data.assets.scripts) {
            AssetManager::RemoveScriptAsset(asset.id);
        }
        for (const auto& asset : m_SceneData.file_data.assets.render_meshes) {
            AssetManager::RemoveRenderMeshAsset(asset.id);
        }
        for (const auto& asset : m_SceneData.file_data.assets.shaders) {
            AssetManager::RemoveShaderAsset(asset.id);
        }
    }
    void EditorLayer::PrepareScene() {
        m_EditorCamera = m_SceneData.scene->CreateEntity("Editor Camera", false);
        m_EditorCamera.AddComponent<TransformComponent>();
        m_EditorCamera.AddComponent<FPSCamera>();
        m_EditorCamera.AddComponent<ScriptComponent>(ScriptEngine::CreateScript(ScriptManager::EditorScript("EditorCameraController")));

        m_SceneData.scene->RegisterSystem<MovingSystem>();
        m_SceneHierarchy.SetActiveScene(m_SceneData.scene);
        m_Inspector.SetScene(m_SceneData.scene);
        m_Inspector.SetActiveEntity(m_SceneHierarchy.GetActiveEntity());
    }

    void EditorLayer::ProcessControlls(float dt) {
        DE_PROFILE_SCOPE("ProcessControlls");

        if (Input::KeyDown(Key::LeftControl)) {
            if (Input::KeyReleased(Key::GraveAccent) && m_SceneData.scene != nullptr) {
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
                    if (m_SceneData.scene) {
                        CloseScene();
                    }
                }
                if (Input::KeyReleased(Key::R)) {
                    if (m_SceneData.scene) {
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