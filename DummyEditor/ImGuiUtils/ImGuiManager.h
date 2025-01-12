#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

#include <vector>

namespace DummyEngine {
    class ImGuiManager {
    public:
        ImGuiManager() = default;

        const std::vector<ImGuiPanel*>& GetPanels();
        void                            SetMenuBar(ImGuiPanel* bar);
        void                            AddPanel(ImGuiPanel* panel);
        void                            OnImGui();

        void LoadEditorResources();

    private:
        void CreateDockingSpace();

        ImGuiPanel*              menu_bar_;
        std::vector<ImGuiPanel*> panels_;
    };
}  // namespace DummyEngine