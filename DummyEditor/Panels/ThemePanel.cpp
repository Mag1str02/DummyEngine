#include "DummyEditor/Panels/ThemePanel.h"

namespace DE {

    void EditorTheme::Apply() const {
        auto& colors                           = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                  = Text;
        colors[ImGuiCol_TextDisabled]          = TextDisabled;
        colors[ImGuiCol_WindowBg]              = Background;
        colors[ImGuiCol_ChildBg]               = Background;
        colors[ImGuiCol_PopupBg]               = Background;
        colors[ImGuiCol_Border]                = Border;
        colors[ImGuiCol_BorderShadow]          = Border;
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
        colors[ImGuiCol_Separator]             = Separator;
        colors[ImGuiCol_SeparatorHovered]      = Separator;
        colors[ImGuiCol_SeparatorActive]       = Separator;
        colors[ImGuiCol_ResizeGrip]            = WidgetInactive;
        colors[ImGuiCol_ResizeGripHovered]     = WidgetHovered;
        colors[ImGuiCol_ResizeGripActive]      = WidgetActive;
        colors[ImGuiCol_Tab]                   = Tab;
        colors[ImGuiCol_TabHovered]            = WidgetHovered;
        colors[ImGuiCol_TabActive]             = Background;
        colors[ImGuiCol_TabUnfocused]          = Tab;
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
    void ThemePanel::AddTheme(const EditorTheme& theme) {}
    void ThemePanel::SetTheme(const std::string& name) {
        if (!m_Themes.contains(name)) {
            return;
        }
        const auto& theme = m_Themes.at(name);
        theme.Apply();
    }
    void ThemePanel::SetDefaultTheme() {
        EditorTheme tmp;
        tmp.Apply();
    }

    void ThemePanel::View() {}
}  // namespace DE
