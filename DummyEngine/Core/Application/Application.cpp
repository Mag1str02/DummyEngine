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
        window_->SetEventCallback([](Event& e) { Application::OnEvent(e); });

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
        layer->event_callback_ = [](Event& e) { Application::OnEvent(e); };
        layer->OnAttach();
        return Unit();
    }
    S_METHOD_IMPL(Unit, OnEvent, (Event & event), (event)) {
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
        while (!should_close_) {
            DE_PROFILER_BEGIN_FRAME();
            DE_PROFILE_SCOPE("Aplication loop");

            frame_end       = glfwGetTime();
            prev_frame_time = frame_end - frame_begin;
            frame_begin     = glfwGetTime();

            Renderer::BeginFrame();
            Input::NewFrame();

            window_->OnUpdate();
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
        event_dispatcher_.AddEventListener<WindowResizeEvent>([this](WindowResizeEvent& event) { OnWindowResize(event); });
        event_dispatcher_.AddEventListener<WindowCloseEvent>([this](WindowCloseEvent& event) { OnWindowClose(event); });
        event_dispatcher_.AddEventListener<SetWindowModeFullscreenEvent>(
            [this](SetWindowModeFullscreenEvent& event) { window_->FullScreen(event.GetMonitorId()); });
        event_dispatcher_.AddEventListener<SetWindowModeWindowedEvent>(
            [this](SetWindowModeWindowedEvent& event) { window_->Windowed(event.GetWidth(), event.GetHeight(), event.GetXPos(), event.GetYPos()); });
        event_dispatcher_.AddEventListener<SetMouseLockEvent>([this](SetMouseLockEvent& event) {
            window_->LockMouse();
            Input::OnEvent(event);
        });
        event_dispatcher_.AddEventListener<SetMouseUnlockEvent>([this](SetMouseUnlockEvent& event) {
            window_->UnlockMouse();
            Input::OnEvent(event);
        });
        event_dispatcher_.AddEventListener<SetMouseLockToggleEvent>([this](SetMouseLockToggleEvent& event) {
            window_->ToggleMouseLock();
            Input::OnEvent(event);
        });

        event_dispatcher_.AddEventListener<KeyPressedEvent>(Input::OnEvent);
        event_dispatcher_.AddEventListener<KeyReleasedEvent>(Input::OnEvent);
        event_dispatcher_.AddEventListener<MouseMovedCallback>(Input::OnEvent);
    }
    void Application::OnWindowResize(WindowResizeEvent& e) {
        Renderer::SetViewport(e.GetWidth(), e.GetHeight());
    }
    void Application::OnWindowClose(WindowCloseEvent&) {
        should_close_ = true;
    }

}  // namespace DummyEngine
