#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DummyEngine {

    class EditorLayer;
    class MenuBar : public ImGuiPanel {
    public:
        MenuBar() : ImGuiPanel("MenuBar") {}
        virtual void OnImGui() override;

    private:
        void ViewMenu();
        void FileMenu();
    };

}  // namespace DummyEngine