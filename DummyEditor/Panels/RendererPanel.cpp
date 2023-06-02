#include "DummyEditor/Panels/RendererPanel.h"

namespace DE {

    void RendererPanel::OnImGui() {
        DE_PROFILE_SCOPE("RendererPanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_PHOTO_CAMERA "  Renderer")) {
                auto scene = m_Scene.lock();
                if (scene) {
                    const float mix_exposure  = 0;
                    const float min_gamma     = 0.01;
                    const float min_threshold = 0.01;
                    const float min_radius    = 0;
                    const U32   min_depth     = 1;
                    const float min_strength  = 0;
                    const float max_strength  = 1;

                    auto& settings = scene->GetRenderer()->settings;

                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("Bloom", settings.bloom);
                    ImGuiUtils::EditProperty("Treshold", ImGuiDataType_Float, &settings.bloom_threshold, 0.01, &settings.bloom_soft_threshold);
                    ImGuiUtils::EditProperty(
                        "SoftTreshold", ImGuiDataType_Float, &settings.bloom_soft_threshold, 0.01, &min_threshold, &settings.bloom_threshold);
                    ImGuiUtils::EditProperty("Radius", ImGuiDataType_Float, &settings.bloom_radius, 0.001, &min_radius);
                    ImGuiUtils::EditProperty("Strength", ImGuiDataType_Float, &settings.bloom_strength, 0.001, &min_strength, &max_strength);
                    ImGuiUtils::EditProperty("Depth", ImGuiDataType_U32, &settings.bloom_depth, 0.01, &min_depth);
                    ImGui::Columns(1);
                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("ToneMapping", settings.gamma_tone_mapping);
                    ImGuiUtils::EditProperty("Exposure", ImGuiDataType_Float, &settings.exposure, 0.01, &mix_exposure);
                    ImGuiUtils::EditProperty("Gamma", ImGuiDataType_Float, &settings.gamma, 0.01, &min_gamma);
                    ImGui::Columns(1);
                    ImGui::Separator();
                }
            }
            ImGui::End();
        }
    }
    void RendererPanel::SetScene(Ref<Scene> scene) {
        m_Scene = scene;
    }
}  // namespace DE