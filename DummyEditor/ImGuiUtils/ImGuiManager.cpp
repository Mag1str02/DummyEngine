#include "DummyEditor/ImGuiUtils/ImGuiManager.h"

namespace DE {

    void ImGuiManager::SetMenuBar(ImGuiItem* bar) {
        m_MenuBar = bar;
    }
    void ImGuiManager::AddPanel(ImGuiPanel* panel) {
        m_Panels.push_back(panel);
    }
    void ImGuiManager::OnImGui() {
        CreateDockingSpace();
        for (auto panel : m_Panels) {
            panel->OnImGui();
        }
    }

    const std::vector<ImGuiPanel*>& ImGuiManager::GetPanels() {
        return m_Panels;
    }

    void ImGuiManager::LoadEditorResources() {
        ImGuiIO& io       = ImGui::GetIO();
        Path     font_dir = Config::GetPath(DE_CFG_FONT_PATH) / "Editor";
        for (const auto& entry : fs::directory_iterator(font_dir)) {
            // TODO: font size to settings
            io.Fonts->AddFontFromFileTTF(entry.path().string().c_str(), 15.0f);
        }
    }

    void ImGuiManager::CreateDockingSpace() {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace",
                     NULL,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar |
                         ImGuiWindowFlags_NoDocking);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        m_MenuBar->OnImGui();
        ImGui::End();
    }
}  // namespace DE