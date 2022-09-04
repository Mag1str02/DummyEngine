#include "Core/Application/Application.h"
#include "Core/Rendering/Renderer/Renderer.h"

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
    void TerminateGLFW() { glfwTerminate(); }

    Application::Application(std::string name) : m_ShouldClose(false)
    {
        DE_PROFILER_BEGIN_FRAME();
        
        DE_ASSERT(s_ApplicationInstance == nullptr, "Creating more than one application at a time.");
        s_ApplicationInstance = this;

        Config::Init();
        Logger::Open(Config::GetPath(DE_CFG_LOG_PATH) / "loading.txt", "loading");
        Logger::Open(Config::GetPath(DE_CFG_LOG_PATH) / "ECS.txt", "ECS");

        InitGLFW();

        m_Window = new Window(WindowState(WindowMode::Windowed, name, 1280, 720));
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

        Renderer::Init(Config::GetRenderAPI());

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);

        SetUpCallbacks();
    }
    Application::~Application()
    {
        delete m_Window;
        for (auto layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
        m_Layers.clear();

        Logger::Close("ECS");
        Logger::Close("loading");
    }

    void Application::PushLayer(Layer * layer)
    {
        m_Layers.push_back(layer);
        layer->m_EventCallback = [this](Event& e) { OnEvent(e); };
        layer->OnAttach();
    }
    void Application::OnEvent(Event & event)
    {
        m_EventDispatcher.Dispatch(event);
        for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
        {
            (*it)->OnEvent(event);
        }
    }
    void Application::Run()
    {
        double frame_begin, frame_end, prev_frame_time = 0.001, fake_time;
        while (!m_ShouldClose)
        {
            frame_end = glfwGetTime();
            prev_frame_time = frame_end - frame_begin;
            frame_begin = glfwGetTime();

            DE_PROFILER_BEGIN_FRAME();

            Renderer::BeginFrame();
            Input::NewFrame();
            m_Window->OnUpdate();
            {
                DE_PROFILE_SCOPE("Layers OnUpdate");

                for (auto layer : m_Layers)
                {
                    layer->OnUpdate(prev_frame_time);
                }
            }
            {
                DE_PROFILE_SCOPE("Layers OnImGuiRender");

                m_ImGuiLayer->BeginFrame();
                for (auto layer : m_Layers)
                {
                    layer->OnImGuiRender();
                }
                m_ImGuiLayer->EndFrame();
            }

            Renderer::EndFrame();
        }
    }

    Application& Application::Get() { return *s_ApplicationInstance; }

    void Application::SetUpCallbacks()
    {
        m_EventDispatcher.AddEventListener<WindowResizeEvent>([this](WindowResizeEvent& event) { OnWindowResize(event); });
        m_EventDispatcher.AddEventListener<WindowCloseEvent>([this](WindowCloseEvent& event) { OnWindowClose(event); });
        m_EventDispatcher.AddEventListener<SetWindowModeFullscreenEvent>([this](SetWindowModeFullscreenEvent& event) { m_Window->FullScreen(event.GetMonitorId()); });
        m_EventDispatcher.AddEventListener<SetWindowModeWindowedEvent>([this](SetWindowModeWindowedEvent& event)
                                                                       { m_Window->Windowed(event.GetWidth(), event.GetHeight(), event.GetXPos(), event.GetYPos()); });
        m_EventDispatcher.AddEventListener<SetMouseLockEvent>(
            [this](SetMouseLockEvent& event)
            {
                m_Window->LockMouse();
                Input::OnEvent(event);
            });
        m_EventDispatcher.AddEventListener<SetMouseUnlockEvent>(
            [this](SetMouseUnlockEvent& event)
            {
                m_Window->UnlockMouse();
                Input::OnEvent(event);
            });
        m_EventDispatcher.AddEventListener<SetMouseLockToggleEvent>(
            [this](SetMouseLockToggleEvent& event)
            {
                m_Window->ToggleMouseLock();
                Input::OnEvent(event);
            });

        m_EventDispatcher.AddEventListener<KeyPressedEvent>(Input::OnEvent);
        m_EventDispatcher.AddEventListener<KeyReleasedEvent>(Input::OnEvent);
        m_EventDispatcher.AddEventListener<MouseMovedCallback>(Input::OnEvent);
    }
    void Application::OnWindowResize(WindowResizeEvent & e) { Renderer::OnWindowResize(e.GetWidth(), e.GetHeight()); }
    void Application::OnWindowClose(WindowCloseEvent & e) { m_ShouldClose = true; }

}  // namespace namespaceRDE
