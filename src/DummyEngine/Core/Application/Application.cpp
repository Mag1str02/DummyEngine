#include "Core/Application/Application.h"

#include "Core/Rendering/Renderer/Renderer.h"
#include "ToolBox/Dev/FrameTimeReader.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{
    static Application* s_ApplicationInstance = nullptr;

    void InitGLFW()
    {
        if (!glfwInit())
        {
            DE_ASSERT(false, "Failed to initialize GLFW.");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Logger::Info("loading", "Initializer", "GLFW initialized.");
    }
    void TerminateGLFW()
    {
        glfwTerminate();
    }

    Application::Application(std::string name)
    {
        DE_ASSERT(s_ApplicationInstance == nullptr, "Creating more than one application at a time.");
        s_ApplicationInstance = this;

        Config::Init();
        Logger::Open(Config::GetPath(DE_CFG_LOG_PATH) / "loading.txt", "loading");
        Logger::Open(Config::GetPath(DE_CFG_LOG_PATH) / "ECS.txt", "ECS");

        InitGLFW();

        m_Window = new Window(WindowState(WindowMode::Windowed, name, 1280, 720, 0));

        Renderer::Init(Config::GetRenderAPI());

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);

    }
    Application::~Application()
    {
        delete m_Window;
        for(auto layer : m_Layers){
            layer->OnDetach();
            delete layer;
        }
        m_Layers.clear();
        
        Logger::Close("ECS");
        Logger::Close("loading");
    }

    void Application::PushLayer(Layer* layer)
    {
        m_Layers.push_back(layer);
        layer->OnAttach();
    }

    void Application::OnLoad() {}
    void Application::OnUpdate(float dt) {}
    void Application::OnShutdown() {}

    void Application::Run()
    {
        OnLoad();
        double frame_begin, frame_end, prev_frame_time = 0.001, fake_time;
        while (!m_Window->ShouldClose())
        {
            frame_end = glfwGetTime();
            prev_frame_time = frame_end - frame_begin;
            frame_begin = glfwGetTime();

            DE_PROFILE_SCOPE("", {
                Renderer::BeginFrame();

                DE_PROFILE_SCOPE("Window Update", m_Window->OnUpdate());
                DE_PROFILE_SCOPE("Application Update Function", OnUpdate(prev_frame_time));
                DE_PROFILE_SCOPE("Layers Update", {
                    for (auto layer : m_Layers)
                    {
                        layer->OnUpdate(prev_frame_time);
                    }
                });
                DE_PROFILE_SCOPE("Layers ImGui Render", {
                    m_ImGuiLayer->BeginFrame();
                    for (auto layer : m_Layers)
                    {
                        layer->OnImGuiRender();
                    }
                    m_ImGuiLayer->EndFrame();
                });

                Renderer::EndFrame();
            });

            DE_FTR_PRINT();
        }
        OnShutdown();
    }

    Application& Application::Get()
    {
        return *s_ApplicationInstance;
    }

}  // namespace DE
