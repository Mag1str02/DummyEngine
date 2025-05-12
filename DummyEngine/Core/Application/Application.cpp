#include "Application.h"

#include "DummyEngine/Core/Application/ImGuiLayer.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Application/Window.h"
#include "DummyEngine/Core/Console/ConsoleLayer.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

#include <GLFW/glfw3.h>

namespace DummyEngine {

    SINGLETON_BASE(Application);
    S_INITIALIZE() {
        // TODO: Customizeble name
        window_ = new Window(WindowState{.Mode = WindowMode::Windowed, .Name = "DummyEngine", .Width = 1280, .Height = 720});
        DE_ASSERT(window_, "Failed to allocate Windows");

        imgui_layer_ = new ImGuiLayer();
        DE_ASSERT(imgui_layer_, "Failed to allocate ImGuiLayer");
        PushLayer(imgui_layer_);

        console_layer_ = new ConsoleLayer();
        DE_ASSERT(console_layer_, "Failed to allocate ConsoleLayer");
        PushLayer(console_layer_);

        SetUpCallbacks();
        return Unit();
    }
    S_TERMINATE() {
        for (auto layer : layers_) {
            layer->OnDetach();
            delete layer;
        }
        layers_.clear();
        delete window_;
        return Unit();
    }

    S_METHOD_IMPL(Unit, PushLayer, (Layer * layer), (layer)) {
        layers_.push_back(layer);
        layer->OnAttach();
        return Unit();
    }
    S_METHOD_IMPL(Unit, OnEvent, (const Event& event), (event)) {
        event_dispatcher_.Dispatch(event);
        for (auto it = layers_.rbegin(); it != layers_.rend(); ++it) {
            (*it)->OnEvent(event);
        }
        return Unit();
    }
    S_METHOD_IMPL(Unit, Run, (), ()) {
        double frame_begin = glfwGetTime();
        double frame_end;
        double prev_frame_time = 0.001;

        DE_PROFILE_SCOPE("Aplication::Run");
        while (!should_close_) {
            DE_PROFILER_BEGIN_FRAME();
            DE_PROFILE_SCOPE("Aplication loop");

            frame_end       = glfwGetTime();
            prev_frame_time = frame_end - frame_begin;
            frame_begin     = glfwGetTime();

            window_->OnUpdate();

            Renderer::BeginFrame();
            Input::NewFrame();

            {
                DE_PROFILE_SCOPE("Layers OnUpdate");

                for (auto layer : layers_) {
                    layer->OnUpdate(prev_frame_time);
                }
            }
            {
                DE_PROFILE_SCOPE("Layers OnImGuiRender");
                Renderer::SetDefaultFrameBuffer();
                imgui_layer_->BeginFrame();
                for (auto layer : layers_) {
                    layer->OnImGuiRender();
                }
                imgui_layer_->EndFrame();
            }

            Renderer::EndFrame();
        }
        return Unit();
    }
    S_METHOD_IMPL(Window&, GetWindow, (), ()) {
        return *window_;
    }

    void Application::SetUpCallbacks() {
        event_dispatcher_.AddEventListener<WindowResizeEvent>([](const WindowResizeEvent& event) {
            Renderer::SetViewport(event.Width, event.Height);  //
        });
        event_dispatcher_.AddEventListener<WindowCloseEvent>([this](const WindowCloseEvent&) { should_close_ = true; });
        event_dispatcher_.AddEventListener<SetWindowModeEvent>([this](const SetWindowModeEvent& event) {
            if (event.Fullscreen) {
                window_->FullScreen(event.MonitorId);
            } else {
                window_->Windowed();
            }
        });
        event_dispatcher_.AddEventListener<SetMouseLockEvent>([this](const SetMouseLockEvent& event) {
            switch (event.Action) {
                case SetMouseLockEvent::Lock: window_->LockMouse(); return;
                case SetMouseLockEvent::UnLock: window_->UnlockMouse(); return;
                case SetMouseLockEvent::Switch: window_->ToggleMouseLock(); return;
                default: DE_ASSERT(false, "Invalid value of action: {}", (U32)event.Action);
            }
        });

        event_dispatcher_.AddEventListener<SetMouseLockEvent>(Input::OnEvent);
        event_dispatcher_.AddEventListener<KeyEvent>(Input::OnEvent);
        event_dispatcher_.AddEventListener<MousePositionEvent>(Input::OnEvent);
    }

}  // namespace DummyEngine
