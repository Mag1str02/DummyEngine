#include "DummyEngine/Core/Application/Application.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {
    SINGLETON_BASE(Application);
    S_INITIALIZE() {
        // TODO: Customizeble name
        m_Window = new Window(WindowState{.mode = WindowMode::Windowed, .name = "DummyEngine", .width = 1280, .height = 720});
        DE_ASSERT(m_Window, "Failed to allocate Windows");
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

        m_ImGuiLayer = new ImGuiLayer();
        DE_ASSERT(m_ImGuiLayer, "Failed to allocate ImGuiLayer");
        PushLayer(m_ImGuiLayer);

        m_ConsoleLayer = new ConsoleLayer();
        DE_ASSERT(m_ConsoleLayer, "Failed to allocate ConsoleLayer");
        PushLayer(m_ConsoleLayer);

        SetUpCallbacks();

        m_Executor = MakeThreadPoolExecutor(THREAD_COUNT);
        return Unit();
    }
    S_TERMINATE() {
        delete m_Window;
        for (auto layer : m_Layers) {
            layer->OnDetach();
            delete layer;
        }
        m_Layers.clear();
        m_Executor->WaitShutdown();
        return Unit();
    }

    S_METHOD_IMPL(Unit, PushLayer, (Layer * layer), (layer)) {
        m_Layers.push_back(layer);
        layer->m_EventCallback = [this](Event& e) { OnEvent(e); };
        layer->OnAttach();
        return Unit();
    }
    S_METHOD_IMPL(Unit, OnEvent, (Event & event), (event)) {
        m_EventDispatcher.Dispatch(event);
        for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it) {
            (*it)->OnEvent(event);
        }
        return Unit();
    }
    S_METHOD_IMPL(Unit, Run, (), ()) {
        double frame_begin = glfwGetTime(), frame_end, prev_frame_time = 0.001, fake_time;
        while (!m_ShouldClose) {
            frame_end       = glfwGetTime();
            prev_frame_time = frame_end - frame_begin;
            frame_begin     = glfwGetTime();

            DE_PROFILER_BEGIN_FRAME();

            Renderer::BeginFrame();
            Input::NewFrame();

            m_Window->OnUpdate();
            {
                DE_PROFILE_SCOPE("Layers OnUpdate");

                for (auto layer : m_Layers) {
                    layer->OnUpdate(prev_frame_time);
                }
            }
            {
                DE_PROFILE_SCOPE("Layers OnImGuiRender");
                Renderer::SetDefaultFrameBuffer();
                m_ImGuiLayer->BeginFrame();
                for (auto layer : m_Layers) {
                    layer->OnImGuiRender();
                }
                m_ImGuiLayer->EndFrame();
            }

            Renderer::EndFrame();
        }
        return Unit();
    }
    S_METHOD_IMPL(Window&, GetWindow, (), ()) {
        return *m_Window;
    }
    S_METHOD_IMPL(Ref<Executor>, GetExecutor, (), ()) {
        return m_Executor;
    }

    void Application::SetUpCallbacks() {
        m_EventDispatcher.AddEventListener<WindowResizeEvent>([this](WindowResizeEvent& event) { OnWindowResize(event); });
        m_EventDispatcher.AddEventListener<WindowCloseEvent>([this](WindowCloseEvent& event) { OnWindowClose(event); });
        m_EventDispatcher.AddEventListener<SetWindowModeFullscreenEvent>(
            [this](SetWindowModeFullscreenEvent& event) { m_Window->FullScreen(event.GetMonitorId()); });
        m_EventDispatcher.AddEventListener<SetWindowModeWindowedEvent>(
            [this](SetWindowModeWindowedEvent& event) { m_Window->Windowed(event.GetWidth(), event.GetHeight(), event.GetXPos(), event.GetYPos()); });
        m_EventDispatcher.AddEventListener<SetMouseLockEvent>([this](SetMouseLockEvent& event) {
            m_Window->LockMouse();
            Input::OnEvent(event);
        });
        m_EventDispatcher.AddEventListener<SetMouseUnlockEvent>([this](SetMouseUnlockEvent& event) {
            m_Window->UnlockMouse();
            Input::OnEvent(event);
        });
        m_EventDispatcher.AddEventListener<SetMouseLockToggleEvent>([this](SetMouseLockToggleEvent& event) {
            m_Window->ToggleMouseLock();
            Input::OnEvent(event);
        });

        m_EventDispatcher.AddEventListener<KeyPressedEvent>(Input::OnEvent);
        m_EventDispatcher.AddEventListener<KeyReleasedEvent>(Input::OnEvent);
        m_EventDispatcher.AddEventListener<MouseMovedCallback>(Input::OnEvent);
    }
    void Application::OnWindowResize(WindowResizeEvent& e) {
        Renderer::SetViewport(e.GetWidth(), e.GetHeight());
    }
    void Application::OnWindowClose(WindowCloseEvent& e) {
        m_ShouldClose = true;
    }

}  // namespace DE
