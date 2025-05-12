#include "ImGuiLayer.h"

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Window.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
// clang-format on

namespace DummyEngine {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(Application::GetInstance().window_->window_, false);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(const Event& event) {
        switch (event.Type()) {
            case EventType::SetMouseLock: {
                const auto& e = event.As<SetMouseLockEvent>();
                switch (e.Action) {
                    case SetMouseLockEvent::Lock: mouse_locked_ = true; break;
                    case SetMouseLockEvent::UnLock: mouse_locked_ = false; break;
                    case SetMouseLockEvent::Switch: mouse_locked_ = !mouse_locked_; break;
                    default: DE_ASSERT(false, "Invalid action {}", (U32)e.Action);
                }
                break;
            }
            case EventType::WindowFocus: {
                const auto& e = event.As<WindowFocusEvent>();
                ImGui_ImplGlfw_WindowFocusCallback(e.Window, e.Focused);
                break;
            }
            case EventType::WindowCursorEntered: {
                const auto& e = event.As<WindowCursorEnteredEvent>();
                ImGui_ImplGlfw_CursorEnterCallback(e.Window, e.Entered);
                break;
            }
            case EventType::MousePosition: {
                if (!mouse_locked_) {
                    const auto& e = event.As<MousePositionEvent>();
                    ImGui_ImplGlfw_CursorPosCallback(e.Window, e.X, e.Y);
                }
                break;
            }
            case EventType::MouseButton: {
                const auto& e = event.As<MouseButtonEvent>();
                ImGui_ImplGlfw_MouseButtonCallback(e.Window, e.Button, e.Action, e.Mods);
                break;
            }
            case EventType::MouseScrolled: {
                const auto& e = event.As<MouseScrolledEvent>();
                ImGui_ImplGlfw_ScrollCallback(e.Window, e.XOffset, e.YOffset);
                break;
            }
            case EventType::Key: {
                const auto& e = event.As<KeyEvent>();
                ImGui_ImplGlfw_KeyCallback(e.Window, e.Key, e.Scancode, e.Action, e.Mods);
                break;
            }
            case EventType::Char: {
                const auto& e = event.As<CharEvent>();
                ImGui_ImplGlfw_CharCallback(e.Window, e.C);
                break;
            }
            case EventType::Monitor: {
                const auto& e = event.As<MonitorEvent>();
                ImGui_ImplGlfw_CharCallback(e.Window, e.Event);
                break;
            }
            default: return;
        };
    }

    void ImGuiLayer::BeginFrame() {
        DE_PROFILE_SCOPE("ImGuiLayer BeginFrame");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::EndFrame() {
        DE_PROFILE_SCOPE("ImGuiLayer EndFrame");

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwSwapInterval(0);
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}  // namespace DummyEngine
