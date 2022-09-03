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

            for (auto [id, transformation] : positions)
            {
                meshes[id].mesh_instance->at<Mat4>(0) = transformation.GetTransform();
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
        m_SceneData.frame_buffer->Resize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
        Renderer::OnWindowResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
        m_SceneData.frame_buffer->Bind();
        if (m_SceneData.scene)
        {
            m_SceneData.scene->GetByName("player").GetComponent<FPSCamera>().SetAspect((double)m_Viewport.GetWidth() / m_Viewport.GetHeight());
            DE_PROFILE_SCOPE("Scene Update", m_SceneData.scene->OnUpdate(dt));
            DE_PROFILE_SCOPE("Scene Render", m_SceneData.scene->Render());
        }

        m_SceneData.frame_buffer->UnBind();
    }
    void EditorLayer::OnImGuiRender()
    {
        ShowDockingSpace();
        m_Viewport.OnImGuiRender(m_SceneData.frame_buffer);
        ImGui::ShowDemoWindow();
    }

    void EditorLayer::ShowDockingSpace()
    {
        static bool p_open = true;

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", &p_open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Scene"))
                {
                    OpenScene(Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
    void EditorLayer::OpenScene(const Path& scene_path)
    {
        SceneLoader::Load(m_SceneData.scene, scene_path);
        m_SceneData.scene->RegisterSystem<MovingSystem>();
    }
}  // namespace DE