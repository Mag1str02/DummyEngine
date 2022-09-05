#include "EditorLayer.h"

namespace DE
{
    class MovingSystem : public System
    {
    public:
        MovingSystem() {}
        virtual std::string GetName() const override
        {
            return "MovingSystem";
        }

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
        m_SceneData.frame_buffer = FrameBuffer::Create({1920, 1080});
        m_SceneData.frame_buffer->AddColorAttachment(TextureFormat::RGBA);
        m_SceneData.frame_buffer->SetDepthAttachment(TextureFormat::DepthStencil);
    }
    void EditorLayer::OnUpdate(float dt)
    {
        DE_PROFILE_SCOPE("EditorLayer OnUpdate");

        ProcessControlls();

        {
            DE_PROFILE_SCOPE("OnViewPortResize");

            m_SceneData.frame_buffer->Resize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            Renderer::OnWindowResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
            m_SceneData.frame_buffer->Bind();
        }

        if (m_SceneData.scene)
        {
            m_SceneData.scene->GetByName("player").GetComponent<FPSCamera>().SetAspect((double)m_Viewport.GetWidth() / m_Viewport.GetHeight());
            m_SceneData.scene->OnUpdate(dt);
            m_SceneData.scene->Render();
        }

        m_SceneData.frame_buffer->UnBind();
    }
    void EditorLayer::OnImGuiRender()
    {
        DE_PROFILE_SCOPE("EditorLayer OnImGuiRender");

        ShowDockingSpace();
        m_Viewport.OnImGuiRender(m_SceneData.frame_buffer);
        m_Profiler.OnImGuiRender();
        m_SceneHierarchy.OnImGuiRender();
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
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar |
                         ImGuiWindowFlags_NoDocking);
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
        SceneLoader::Load(m_SceneData.scene, scene_path);
        m_SceneData.scene->RegisterSystem<MovingSystem>();
        m_SceneHierarchy.SetActiveScene(m_SceneData.scene);
    }
    void EditorLayer::SaveScene(const Path& path)
    {
        SceneLoader::Save(m_SceneData.scene, path);
    }

    void EditorLayer::ProcessControlls()
    {
        DE_PROFILE_SCOPE("ProcessControlls");

        if (Input::KeyDown(GLFW_KEY_LEFT_CONTROL))
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
}  // namespace DE