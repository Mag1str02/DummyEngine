#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include "DummyEngine/Core/Application/ImGuiLayer.h"
#include "DummyEngine/Core/Application/Application.h"

namespace DE
{

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(Application::Get().m_Window->m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        SetDarkThemeColors();
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::BeginFrame()
    {
        DE_PROFILE_SCOPE("ImGuiLayer BeginFrame");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::EndFrame()
    {
        DE_PROFILE_SCOPE("ImGuiLayer EndFrame");

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwSwapInterval(0);
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto& colors              = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = {0.1f, 0.105f, 0.11f, 1.0f};

        colors[ImGuiCol_Header]        = {0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive]  = {0.15f, 0.1505f, 0.151f, 1.0f};

        colors[ImGuiCol_Button]        = {0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive]  = {0.15f, 0.1505f, 0.151f, 1.0f};

        colors[ImGuiCol_FrameBg]        = {0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive]  = {0.15f, 0.1505f, 0.151f, 1.0f};

        colors[ImGuiCol_Tab]                = {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabHovered]         = {0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive]          = {0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused]       = {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = {0.2f, 0.205f, 0.21f, 1.0f};

        colors[ImGuiCol_TitleBg]          = {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgActive]    = {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_DragDropTarget]   = {0.5f, 0.5f, 0.5f, 1.0f};

        colors[ImGuiCol_DockingPreview] = {0.5f, 0.5f, 0.5f, 1.0f};
    };
}  // namespace DE

//*Enums for colors

// ImGuiCol_Text,
// ImGuiCol_TextDisabled,
// ImGuiCol_WindowBg,              // Background of normal windows
// ImGuiCol_ChildBg,               // Background of child windows
// ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
// ImGuiCol_Border,
// ImGuiCol_BorderShadow,
// ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
// ImGuiCol_FrameBgHovered,
// ImGuiCol_FrameBgActive,
// ImGuiCol_TitleBg,
// ImGuiCol_TitleBgActive,
// ImGuiCol_TitleBgCollapsed,
// ImGuiCol_MenuBarBg,
// ImGuiCol_ScrollbarBg,
// ImGuiCol_ScrollbarGrab,
// ImGuiCol_ScrollbarGrabHovered,
// ImGuiCol_ScrollbarGrabActive,
// ImGuiCol_CheckMark,
// ImGuiCol_SliderGrab,
// ImGuiCol_SliderGrabActive,
// ImGuiCol_Button,
// ImGuiCol_ButtonHovered,
// ImGuiCol_ButtonActive,
// ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
// ImGuiCol_HeaderHovered,
// ImGuiCol_HeaderActive,
// ImGuiCol_Separator,
// ImGuiCol_SeparatorHovered,
// ImGuiCol_SeparatorActive,
// ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
// ImGuiCol_ResizeGripHovered,
// ImGuiCol_ResizeGripActive,
// ImGuiCol_Tab,                   // TabItem in a TabBar
// ImGuiCol_TabHovered,
// ImGuiCol_TabActive,
// ImGuiCol_TabUnfocused,
// ImGuiCol_TabUnfocusedActive,
// ImGuiCol_DockingPreview,        // Preview overlay color when about to docking something
// ImGuiCol_DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
// ImGuiCol_PlotLines,
// ImGuiCol_PlotLinesHovered,
// ImGuiCol_PlotHistogram,
// ImGuiCol_PlotHistogramHovered,
// ImGuiCol_TableHeaderBg,         // Table header background
// ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
// ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
// ImGuiCol_TableRowBg,            // Table row background (even rows)
// ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
// ImGuiCol_TextSelectedBg,
// ImGuiCol_DragDropTarget,        // Rectangle highlighting a drop target
// ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
// ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
// ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
// ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
// ImGuiCol_COUNT
