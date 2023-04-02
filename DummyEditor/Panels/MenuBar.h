#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    class EditorLayer;
    class MenuBar : public ImGuiItem {
    public:
        MenuBar() : ImGuiItem("MenuBar") {}
        void AttachToEditor(EditorLayer* editor);
        void OnImGui() override;

    private:
        void ViewMenu();
        void FileMenu();

        EditorLayer* m_Editor;
    };
}  // namespace DE