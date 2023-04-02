#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    class MenuBar;
    class ImGuiManager {
    public:
        ImGuiManager() = default;

        const std::vector<ImGuiPanel*>& GetPanels();
        void                            SetMenuBar(ImGuiItem* bar);
        void                            AddPanel(ImGuiPanel* panel);
        void                            OnImGui();

        void LoadEditorResources();

    private:
        void                     CreateDockingSpace();
        ImGuiItem*               m_MenuBar;
        std::vector<ImGuiPanel*> m_Panels;
    };
}  // namespace DE