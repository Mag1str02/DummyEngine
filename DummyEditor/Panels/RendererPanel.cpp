#include "DummyEditor/Panels/RendererPanel.h"

namespace DE {

    void RendererPanel::OnImGui() {
        DE_PROFILE_SCOPE("RendererPanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin("  Renderer")) {
                auto scene = m_Scene.lock();
                if (scene) {
                    const float mix_exposure  = 0;
                    const float min_gamma     = 0.01;
                    const float min_threshold = 0.01;
                    const float min_radius    = 0;
                    const U32   min_depth     = 1;
                    const float min_strength  = 0;
                    const float max_strength  = 1;

                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("Bloom", m_Bloom);
                    ImGuiUtils::EditProperty("Treshold", ImGuiDataType_Float, &m_BloomTreshold, 0.01, &min_threshold);
                    ImGuiUtils::EditProperty("SoftTreshold", ImGuiDataType_Float, &m_BloomSoftTreshold, 0.01, &min_threshold, &m_BloomTreshold);
                    ImGuiUtils::EditProperty("Radius", ImGuiDataType_Float, &m_BloomRadius, 0.001, &min_radius);
                    ImGuiUtils::EditProperty("Strength", ImGuiDataType_Float, &m_BloomStrength, 0.001, &min_strength, &max_strength);
                    ImGuiUtils::EditProperty("Depth", ImGuiDataType_U32, &m_BloomDepth, 0.01, &min_depth);
                    ImGui::Columns(1);
                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("ToneMapping", m_GammaHDR);
                    ImGuiUtils::EditProperty("Exposure", ImGuiDataType_Float, &m_Exposure, 0.01, &mix_exposure);
                    ImGuiUtils::EditProperty("Gamma", ImGuiDataType_Float, &m_Gamma, 0.01, &min_gamma);
                    ImGui::Columns(1);
                    ImGui::Separator();

                    scene->GetRenderer()->Bloom             = m_Bloom;
                    scene->GetRenderer()->BloomTreshold     = m_BloomTreshold;
                    scene->GetRenderer()->BloomSoftTreshold = m_BloomSoftTreshold;
                    scene->GetRenderer()->BloomRadius       = m_BloomRadius;
                    scene->GetRenderer()->BloomDepth        = m_BloomDepth;
                    scene->GetRenderer()->BloomStrength     = m_BloomStrength;

                    scene->GetRenderer()->Gamma    = m_Gamma;
                    scene->GetRenderer()->Exposure = m_Exposure;
                    scene->GetRenderer()->GammaHDR = m_GammaHDR;
                }
            }
            ImGui::End();
        }
    }
    void RendererPanel::SetScene(Ref<Scene> scene) {
        m_Scene = scene;
    }
}  // namespace DE