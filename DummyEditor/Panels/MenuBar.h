#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class EditorLayer;
    class MenuBar : public ImGuiPanel {
    public:
        MenuBar() : ImGuiPanel("MenuBar") {}
        virtual void OnImGui() override;

    private:
        void ViewMenu();
        void FileMenu();
    };
}  // namespace DE