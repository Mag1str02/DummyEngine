#include "DummyEditor/Panels/MenuBar.h"

#include "DummyEditor/EditorLayer.h"

namespace DE {
    void MenuBar::AttachToEditor(EditorLayer* editor) {
        m_Editor = editor;
        m_Editor->m_ImGuiManager.SetMenuBar(this);
    }
    void MenuBar::OnImGui() {
        DE_PROFILE_SCOPE("MenuBar OnImGui");

        if (ImGui::BeginMenuBar()) {
            FileMenu();
            ViewMenu();
            ImGui::EndMenuBar();
        }
    }
    void MenuBar::FileMenu() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Create Scene")) {
                m_Editor->ActionCreateScene();
            }
            if (ImGui::MenuItem("Open Scene")) {
                m_Editor->ActionOpenScene();
            }
            if (ImGui::MenuItem("Save Scene")) {
                m_Editor->ActionSaveScene();
            }
            if (ImGui::MenuItem("Close Scene")) {
                m_Editor->ActionCloseScene();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                WindowCloseEvent event;
                m_Editor->BroadcastEvent(event);
            }
            ImGui::EndMenu();
        }
    }
    void MenuBar::ViewMenu() {
        if (ImGui::BeginMenu("View")) {
            int cnt = 0;
            for (auto panel : m_Editor->m_ImGuiManager.GetPanels()) {
                ImGuiUtils::ScopedID id(++cnt);
                ImGui::MenuItem(panel->GetName().c_str(), NULL, &panel->GetController());
            }
            ImGui::EndMenu();
        }
    }
}  // namespace DE