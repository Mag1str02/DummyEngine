#include "DummyEditor/Panels/MenuBar.h"

#include "DummyEditor/EditorLayer.h"

namespace DE {
    void MenuBar::OnImGui() {
        DE_PROFILE_SCOPE("MenuBar OnImGui");

        if (ImGui::BeginMenuBar()) {
            FileMenu();
            ViewMenu();
            ImGui::EndMenuBar();
        }
    }
    void MenuBar::FileMenu() {
        auto& editor = EditorLayer::Get();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Create Scene")) {
                editor.ActionCreateScene();
            }
            if (ImGui::MenuItem("Open Scene")) {
                editor.ActionOpenScene();
            }
            if (ImGui::MenuItem("Save Scene")) {
                editor.ActionSaveScene();
            }
            if (ImGui::MenuItem("Close Scene")) {
                editor.ActionCloseScene();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                WindowCloseEvent event;
                editor.BroadcastEvent(event);
            }
            ImGui::EndMenu();
        }
    }
    void MenuBar::ViewMenu() {
        auto& editor = EditorLayer::Get();
        if (ImGui::BeginMenu("View")) {
            int cnt = 0;
            for (auto panel : editor.GetImGuiManager().GetPanels()) {
                ImGuiUtils::ScopedID id(++cnt);
                ImGui::MenuItem(panel->GetName().c_str(), NULL, &panel->GetController());
            }
            ImGui::EndMenu();
        }
    }
}  // namespace DE