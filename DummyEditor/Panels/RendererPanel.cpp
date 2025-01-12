#include "RendererPanel.h"

#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

#include <imgui.h>

namespace DummyEngine {

    void RendererPanel::OnImGui() {
        DE_PROFILE_SCOPE("RendererPanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_PHOTO_CAMERA "  Renderer")) {
                auto scene = scene_.lock();
                if (scene) {
                    const float mix_exposure  = 0;
                    const float min_gamma     = 0.01;
                    const float min_threshold = 0.01;
                    const float min_radius    = 0;
                    const U32   min_depth     = 1;
                    const float min_strength  = 0;
                    const float max_strength  = 1;

                    auto& settings = scene->GetRenderer()->Settings;

                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("Bloom", settings.Bloom);
                    ImGuiUtils::EditProperty("Treshold", ImGuiDataType_Float, &settings.BloomThreshold, 0.01, &settings.BloomSoftThreshold);
                    ImGuiUtils::EditProperty(
                        "SoftTreshold", ImGuiDataType_Float, &settings.BloomSoftThreshold, 0.01, &min_threshold, &settings.BloomThreshold);
                    ImGuiUtils::EditProperty("Radius", ImGuiDataType_Float, &settings.BloomRadius, 0.001, &min_radius);
                    ImGuiUtils::EditProperty("Strength", ImGuiDataType_Float, &settings.BloomStrength, 0.001, &min_strength, &max_strength);
                    ImGuiUtils::EditProperty("Depth", ImGuiDataType_U32, &settings.BloomDepth, 0.01, &min_depth);
                    ImGui::Columns(1);
                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("ToneMapping", settings.GammaToneMapping);
                    ImGuiUtils::EditProperty("Exposure", ImGuiDataType_Float, &settings.Exposure, 0.01, &mix_exposure);
                    ImGuiUtils::EditProperty("Gamma", ImGuiDataType_Float, &settings.Gamma, 0.01, &min_gamma);
                    ImGui::Columns(1);
                    ImGui::Separator();
                    ImGui::Columns(2);
                    ImGuiUtils::EditProperty("DirectionalShadowMap", settings.UseDirectionalShadowMap);
                    ImGuiUtils::EditProperty("PointShadows", settings.UsePointShadows);
                    ImGui::Columns(1);
                    ImGui::Separator();
                }
            }
            ImGui::End();
        }
    }
    void RendererPanel::SetScene(Ref<Scene> scene) {
        scene_ = scene;
    }

}  // namespace DummyEngine