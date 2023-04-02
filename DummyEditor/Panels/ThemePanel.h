#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    struct EditorTheme {
        ImVec4 Text            = {1.0f, 1.0f, 1.0f, 1.0f};
        ImVec4 TextDisabled    = {0.15f, 0.1505f, 0.151f, 1.0f};
        ImVec4 WidgetInactive  = {0.15f, 0.1505f, 0.151f, 1.0f};
        ImVec4 WidgetHovered   = {0.3f, 0.305f, 0.31f, 1.0f};
        ImVec4 WidgetActive    = {0.15f, 0.1505f, 0.151f, 1.0f};
        ImVec4 Background      = U32ToColor(0xFF242424);
        ImVec4 FieldBackground = {0.2f, 0.205f, 0.21f, 1.0f};
        ImVec4 Border          = {0.1f, 0.105f, 0.11f, 1.0f};
        ImVec4 Separator       = {0.0f, 0.0f, 0.0f, 0.0f};
        ImVec4 Button          = {0.2f, 0.205f, 0.21f, 1.0f};
        ImVec4 Header          = {0.15f, 0.1505f, 0.151f, 1.0f};
        ImVec4 TitleBar        = {0.15f, 0.1505f, 0.151f, 1.0f};
        ImVec4 MenuBar         = {0.0f, 0.0f, 0.0f, 0.0f};
        ImVec4 Tab             = {0.15f, 0.1505f, 0.151f, 1.0f};
        ImVec4 CheckMark       = {1.0f, 1.0f, 1.0f, 1.0f};
        ImVec4 Default         = {0.0f, 1.0f, 0.0f, 1.0f};

        void Apply() const;
    };
    class ThemePanel : public ImGuiPanel {
    public:
        ThemePanel() : ImGuiPanel("ThemePanel") { SetDefaultTheme(); }

        void AddTheme(const EditorTheme& theme);
        void SetTheme(const std::string& name);
        void SetDefaultTheme();

        virtual void View() override;

    private:
        std::unordered_map<std::string, EditorTheme> m_Themes;
    };
}  // namespace DE
   //*Enums for colors
