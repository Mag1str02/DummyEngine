#include "EditorLayer.h"

namespace DE
{
    class MovingSystem : public System
    {
    public:
        MovingSystem() {}
        virtual std::string GetName() const override { return "MovingSystem"; }

        void Update(double dt) override
        {
            auto& positions = GetComponentArray<TransformComponent>();
            auto& meshes = GetComponentArray<RenderMeshComponent>();
            auto& light_sources = GetComponentArray<LightSource>();

            for (auto [id, mesh] : meshes)
            {
                mesh.mesh_instance->at<Mat4>(0) = positions[id].GetTransform();
            }

            for (auto [entity_id, light_source] : light_sources)
            {
                if (positions.HasComponent(entity_id))
                {
                    light_source.position = positions[entity_id].translation;
                }
            }
        }
    };

    EditorLayer::EditorLayer() : Layer("EditorLayer") {}

    void EditorLayer::OnAttach()
    {
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
    void EditorLayer::OnUpdate(float dt)
    {
        DE_PROFILE_SCOPE("EditorLayer OnUpdate");

        ProcessControlls(dt);

        {
            DE_PROFILE_SCOPE("OnViewPortResize");

            m_SceneData.m_FrameBuffer->Resize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            Renderer::OnWindowResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            m_SceneData.m_FrameBuffer->Bind();
        }

        if (m_SceneData.m_Scene)
        {
            m_SceneData.m_Scene->OnViewPortResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            m_SceneData.m_Scene->OnUpdate(dt);
            m_SceneData.m_Scene->Render();
        }

        m_SceneData.m_FrameBuffer->UnBind();
    }
    void EditorLayer::OnImGuiRender()
    {
        DE_PROFILE_SCOPE("EditorLayer OnImGuiRender");

        if (m_SceneData.m_Scene)
        {
            m_Inspector.SetCurrentEntity(m_SceneHierarchy.GetActiveEntity());
        }

        ShowDockingSpace();

        m_Panels.OnImGuiRender();

        {
            DE_PROFILE_SCOPE("Demo Window");

            ImGui::ShowDemoWindow();
        }
    }

    //*~~~EditorGUI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void EditorLayer::ShowDockingSpace()
    {
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
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        ShowDockingSpaceTabBar();

        ImGui::End();
    }
    void EditorLayer::ShowDockingSpaceTabBar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Scene"))
                {
                    OpenSceneDialog();
                }
                if (ImGui::MenuItem("Save Scene"))
                {
                    SaveSceneDialog();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit"))
                {
                    WindowCloseEvent event;
                    BroadcastEvent(event);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
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

    void EditorLayer::OpenSceneDialog()
    {
        Path path = FileDialogs::OpenFile("Dummy Engine Scene (*.yml)\0*.yml\0\0");
        if (path != Path())
        {
            OpenScene(path);
        }
    }
    void EditorLayer::SaveSceneDialog()
    {
        Path path = FileDialogs::SaveFile("Dummy Engine Scene (*.yml)\0*.yml\0\0");
        if (path != Path())
        {
            SaveScene(path);
        }
    }
    void EditorLayer::OpenScene(const Path& scene_path)
    {
        SceneLoader::Load(m_SceneData.m_Scene, scene_path);
        m_SceneData.m_Scene->RegisterSystem<MovingSystem>();
        m_SceneHierarchy.SetActiveScene(m_SceneData.m_Scene);
    }
    void EditorLayer::SaveScene(const Path& path) { SceneLoader::Save(m_SceneData.m_Scene, path); }

    void EditorLayer::ProcessControlls(float dt)
    {
        DE_PROFILE_SCOPE("ProcessControlls");

        if (Input::KeyDown(GLFW_KEY_LEFT_CONTROL))
        {
            if (Input::KeyReleased(GLFW_KEY_GRAVE_ACCENT) && m_SceneData.m_Scene != nullptr)
            {
                m_State.m_InputState = (m_State.m_InputState == InputState::ViewPort ? InputState::NonSpecified : InputState::ViewPort);
                SetMouseLockToggleEvent event;
                BroadcastEvent(event);
            }
            if (m_State.m_InputState != InputState::ViewPort)
            {
                if (Input::KeyReleased(GLFW_KEY_O))
                {
                    OpenSceneDialog();
                }
                if (Input::KeyReleased(GLFW_KEY_S))
                {
                    SaveSceneDialog();
                }
            }
        }
        switch (m_State.m_InputState)
        {
            case InputState::ViewPort: {
                auto& camera = m_SceneData.m_Scene->GetCamera();
                float speed = 15;
                float sensitivity = 0.07;

                camera.RotateY(Input::CursorXOffset() * sensitivity);
                camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

                if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT))
                {
                    speed = 100.0f;
                }
                if (Input::KeyDown(GLFW_KEY_ESCAPE))
                {
                    WindowCloseEvent event;
                    BroadcastEvent(event);
                }
                if (Input::KeyDown(GLFW_KEY_S))
                {
                    camera.MoveInLocal(Vec3(0.0f, 0.0f, -1.0f) * speed * dt);
                }
                if (Input::KeyDown(GLFW_KEY_W))
                {
                    camera.MoveInLocal(Vec3(0.0f, 0.0f, 1.0f) * speed * dt);
                }
                if (Input::KeyDown(GLFW_KEY_D))
                {
                    camera.MoveInLocal(Vec3(1.0f, 0.0f, 0.0f) * speed * dt);
                }
                if (Input::KeyDown(GLFW_KEY_A))
                {
                    camera.MoveInLocal(Vec3(-1.0f, 0.0f, 0.0f) * speed * dt);
                }
                if (Input::KeyDown(GLFW_KEY_SPACE))
                {
                    camera.MoveInWorld(Vec3(0.0f, 1.0f, 0.0f) * speed * dt);
                }
                if (Input::KeyDown(GLFW_KEY_C))
                {
                    camera.MoveInWorld(Vec3(0.0f, -1.0f, 0.0f) * speed * dt);
                }
                break;
            }
        }
    }
}  // namespace DE