#include "ThemePanel.h"

#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

namespace DummyEngine {

    ThemePanel::ThemePanel() : ImGuiPanel("ThemePanel") {
        SetDefaultTheme();
        SetDefaultStyle();
    }
    void ThemePanel::SetTheme(const EditorTheme& theme) {
        active_theme_ = theme;
        active_theme_.Apply();
    }
    void ThemePanel::SetDefaultTheme() {
        active_theme_ = EditorTheme();
        active_theme_.Apply();
    }

    void ThemePanel::OnImGui() {
        DE_PROFILE_SCOPE("ThemePanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_BRUSH "  ThemePanel")) {
                ImGui::Separator();
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Background", active_theme_.Background, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("FieldBackground", active_theme_.FieldBackground, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("TitleBar", active_theme_.TitleBar, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("MenuBar", active_theme_.MenuBar, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Header", active_theme_.Header, ImGuiUtils::PropertyType::Color);

                ImGuiUtils::EditProperty("Text", active_theme_.Text, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("TextDisabled", active_theme_.TextDisabled, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("WidgetInactive", active_theme_.WidgetInactive, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("WidgetHovered", active_theme_.WidgetHovered, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("WidgetActive", active_theme_.WidgetActive, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Button", active_theme_.Button, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("CheckMark", active_theme_.CheckMark, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Default", active_theme_.Default, ImGuiUtils::PropertyType::Color);
                ImGui::Columns(1);
                ImGui::Separator();
                active_theme_.Apply();
            }
            ImGui::End();
        }
    }
    void ThemePanel::SetDefaultStyle() {
        auto& style            = ImGui::GetStyle();
        style.WindowPadding    = {5, 5};
        style.FramePadding     = {5, 2};
        style.ItemSpacing      = {5, 2};
        style.ItemInnerSpacing = {2, 2};
        style.IndentSpacing    = 20;
        style.ScrollbarSize    = 15;
        style.GrabMinSize      = 15;

        style.WindowBorderSize = 0;
        style.ChildBorderSize  = 0;
        style.PopupBorderSize  = 1;
        style.FrameBorderSize  = 0;
        style.TabBorderSize    = 0;

        style.WindowRounding    = 3;
        style.ChildRounding     = 0;
        style.FrameRounding     = 3;
        style.PopupRounding     = 3;
        style.GrabRounding      = 3;
        style.ScrollbarRounding = 10;
        style.TabRounding       = 7;

        style.WindowTitleAlign         = {0.5, 0.5};
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.ColorButtonPosition      = ImGuiDir_Right;
        style.ButtonTextAlign          = {0.5, 0.5};
        style.SelectableTextAlign      = {0, 0};
        style.SeparatorTextBorderSize  = 1;
        style.SeparatorTextAlign       = {0.5, 0.5};
        style.SeparatorTextPadding     = {5, 5};
    }

    void EditorTheme::Apply() const {
        auto& colors                           = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                  = Text;
        colors[ImGuiCol_TextDisabled]          = TextDisabled;
        colors[ImGuiCol_WindowBg]              = Background;
        colors[ImGuiCol_ChildBg]               = Background;
        colors[ImGuiCol_PopupBg]               = Background;
        colors[ImGuiCol_Border]                = MenuBar;
        colors[ImGuiCol_BorderShadow]          = MenuBar;
        colors[ImGuiCol_FrameBg]               = FieldBackground;
        colors[ImGuiCol_FrameBgHovered]        = WidgetHovered;
        colors[ImGuiCol_FrameBgActive]         = WidgetActive;
        colors[ImGuiCol_TitleBg]               = TitleBar;
        colors[ImGuiCol_TitleBgActive]         = TitleBar;
        colors[ImGuiCol_TitleBgCollapsed]      = TitleBar;
        colors[ImGuiCol_MenuBarBg]             = MenuBar;
        colors[ImGuiCol_ScrollbarBg]           = Background;
        colors[ImGuiCol_ScrollbarGrab]         = WidgetInactive;
        colors[ImGuiCol_ScrollbarGrabHovered]  = WidgetHovered;
        colors[ImGuiCol_ScrollbarGrabActive]   = WidgetActive;
        colors[ImGuiCol_CheckMark]             = WidgetActive;
        colors[ImGuiCol_SliderGrab]            = WidgetInactive;
        colors[ImGuiCol_SliderGrabActive]      = WidgetActive;
        colors[ImGuiCol_Button]                = Button;
        colors[ImGuiCol_ButtonHovered]         = WidgetHovered;
        colors[ImGuiCol_ButtonActive]          = WidgetActive;
        colors[ImGuiCol_Header]                = Header;
        colors[ImGuiCol_HeaderHovered]         = WidgetHovered;
        colors[ImGuiCol_HeaderActive]          = WidgetActive;
        colors[ImGuiCol_Separator]             = MenuBar;
        colors[ImGuiCol_SeparatorHovered]      = MenuBar;
        colors[ImGuiCol_SeparatorActive]       = MenuBar;
        colors[ImGuiCol_ResizeGrip]            = WidgetInactive;
        colors[ImGuiCol_ResizeGripHovered]     = WidgetHovered;
        colors[ImGuiCol_ResizeGripActive]      = WidgetActive;
        colors[ImGuiCol_Tab]                   = TitleBar;
        colors[ImGuiCol_TabHovered]            = WidgetHovered;
        colors[ImGuiCol_TabActive]             = Background;
        colors[ImGuiCol_TabUnfocused]          = TitleBar;
        colors[ImGuiCol_TabUnfocusedActive]    = Background;
        colors[ImGuiCol_DockingPreview]        = WidgetActive;
        colors[ImGuiCol_DockingEmptyBg]        = WidgetHovered;
        colors[ImGuiCol_PlotLines]             = Default;
        colors[ImGuiCol_PlotLinesHovered]      = Default;
        colors[ImGuiCol_PlotHistogram]         = Default;
        colors[ImGuiCol_PlotHistogramHovered]  = Default;
        colors[ImGuiCol_TableHeaderBg]         = Default;
        colors[ImGuiCol_TableBorderStrong]     = Default;
        colors[ImGuiCol_TableBorderLight]      = Default;
        colors[ImGuiCol_TableRowBg]            = Default;
        colors[ImGuiCol_TableRowBgAlt]         = Default;
        colors[ImGuiCol_TextSelectedBg]        = WidgetActive;
        colors[ImGuiCol_DragDropTarget]        = WidgetActive;
        colors[ImGuiCol_NavHighlight]          = Default;
        colors[ImGuiCol_NavWindowingHighlight] = Default;
        colors[ImGuiCol_NavWindowingDimBg]     = Default;
        colors[ImGuiCol_ModalWindowDimBg]      = Default;
    }
}  // namespace DummyEngine
