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

                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("Bloom", m_Bloom);
                    ImGuiUtils::EditProperty("BrightnessTreshold", ImGuiDataType_Float, &m_BrightnessTreshold, 0.01, &min_threshold);
                    ImGui::Columns(1);
                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("ToneMapping", m_GammaHDR);
                    ImGuiUtils::EditProperty("Exposure", ImGuiDataType_Float, &m_Exposure, 0.01, &mix_exposure);
                    ImGuiUtils::EditProperty("Gamma", ImGuiDataType_Float, &m_Gamma, 0.01, &min_gamma);
                    ImGui::Columns(1);
                    ImGui::Separator();

                    scene->GetRenderer()->BrightnessTreshold = m_BrightnessTreshold;
                    scene->GetRenderer()->Gamma              = m_Gamma;
                    scene->GetRenderer()->Bloom              = m_Bloom;
                    scene->GetRenderer()->Exposure           = m_Exposure;
                    scene->GetRenderer()->GammaHDR           = m_GammaHDR;
                }
            }
            ImGui::End();
        }
    }
    void RendererPanel::SetScene(Ref<Scene> scene) {
        m_Scene = scene;
    }
}  // namespace DE