#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    class ImGuiManager {
    public:
        ImGuiManager() = default;

        const std::vector<ImGuiPanel*>& GetPanels();
        void                            SetMenuBar(ImGuiPanel* bar);
        void                            AddPanel(ImGuiPanel* panel);
        void                            OnImGui();

        void LoadEditorResources();

    private:
        void                     CreateDockingSpace();
        ImGuiPanel*              m_MenuBar;
        std::vector<ImGuiPanel*> m_Panels;
    };
}  // namespace DE