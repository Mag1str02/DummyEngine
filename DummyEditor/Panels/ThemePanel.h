#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    struct EditorTheme {
        std::string Name;

        ImVec4 Background      = ImGuiUtils::U32ToColor(0xFF323232);
        ImVec4 FieldBackground = ImGuiUtils::U32ToColor(0xFF2A2A2A);
        ImVec4 TitleBar        = ImGuiUtils::U32ToColor(0xFF282828);
        ImVec4 MenuBar         = ImGuiUtils::U32ToColor(0xFF202020);
        ImVec4 Header          = ImGuiUtils::U32ToColor(0xFF3E3E3E);

        ImVec4 Text           = ImGuiUtils::U32ToColor(0xFFFFFFFF);
        ImVec4 TextDisabled   = ImGuiUtils::U32ToColor(0xFF797979);
        ImVec4 WidgetInactive = ImGuiUtils::U32ToColor(0xFF3E3E3E);
        ImVec4 WidgetHovered  = ImGuiUtils::U32ToColor(0xFF4D4D4D);
        ImVec4 WidgetActive   = ImGuiUtils::U32ToColor(0xFF4D4D4D);
        ImVec4 Button         = ImGuiUtils::U32ToColor(0xFF585858);
        ImVec4 CheckMark      = ImGuiUtils::U32ToColor(0xFF597092);
        ImVec4 Default        = ImGuiUtils::U32ToColor(0xFF00FF00);

        void Apply() const;
    };
    class ThemePanel : public ImGuiPanel {
    public:
        ThemePanel();

        void               SetTheme(const EditorTheme& name);
        void               SetDefaultTheme();
        const EditorTheme& GetCurrentTheme();

        virtual void OnImGui() override;

    private:
        void SetDefaultStyle();

        EditorTheme m_ActiveTheme;
    };
}  // namespace DE
   //*Enums for colors
