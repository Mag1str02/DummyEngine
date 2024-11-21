#include "ImGuiManager.h"

#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Utils/Types/Types.h"

#include <imgui.h>

namespace DummyEngine {
    void ImGuiManager::SetMenuBar(ImGuiPanel* bar) {
        menu_bar_ = bar;
    }
    void ImGuiManager::AddPanel(ImGuiPanel* panel) {
        panels_.push_back(panel);
    }
    void ImGuiManager::OnImGui() {
        CreateDockingSpace();
        for (auto panel : panels_) {
            panel->OnImGui();
        }
    }

    const std::vector<ImGuiPanel*>& ImGuiManager::GetPanels() {
        return panels_;
    }

    void ImGuiManager::LoadEditorResources() {
        ImGuiIO& io             = ImGui::GetIO();
        Path     font_dir       = Config::Get().ExecutablePath / "Editor" / "Fonts";
        Path     icon_font_path = Config::Get().ExecutablePath / "Editor" / "Icons" / "IconsMaterialDesign.ttf";
        for (const auto& entry : fs::directory_iterator(font_dir)) {
            // TODO: font size to settings
            io.Fonts->AddFontFromFileTTF(entry.path().string().c_str(), ImGuiUtils::Constants::kBasicFontSize);
            static constexpr ImWchar kIconsRanges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};  // do not remove static  : - )
            ImFontConfig             icons_config;
            icons_config.MergeMode   = true;
            icons_config.PixelSnapH  = true;
            icons_config.GlyphOffset = {0.f, 4.f};
            io.Fonts->AddFontFromFileTTF(icon_font_path.string().c_str(), ImGuiUtils::Constants::kBasicFontSize + 1, &icons_config, kIconsRanges);
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
        menu_bar_->OnImGui();
        ImGui::End();
    }
}  // namespace DummyEngine