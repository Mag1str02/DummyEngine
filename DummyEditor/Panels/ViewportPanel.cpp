#include "ViewportPanel.h"

#include "DummyEditor/EditorLayer.h"

#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"

namespace DummyEngine {

    void ViewportPanel::OnImGui() {
        DE_PROFILE_SCOPE("ViewportPanel OnImGui");
        if (m_Controller) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            if (ImGui::Begin((ICON_MD_TV + m_Name).c_str())) {
                m_ViewportSize = ImGui::GetContentRegionAvail();
                auto buffer    = m_FrameBuffer.lock();
                if (buffer) {
                    if (m_UseDepthAttachment) {
                        ImGui::Image(buffer->GetDepthAttachment()->GetRendererId(), m_ViewportSize, {0, 1}, {1, 0});
                    } else {
                        ImGui::Image(buffer->GetColorAttachment(0)->GetRendererId(), m_ViewportSize, {0, 1}, {1, 0});
                        ToolPanel();
                    }
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
    }

    void ViewportPanel::ToolPanel() {
        using State     = EditorLayer::SceneState;
        auto& editor    = EditorLayer::Get();
        auto& resources = editor.GetResources();
        State state     = editor.GetSceneState();

        const ImVec2 button_size    = {32, 32};
        const ImVec2 button_padding = {15, 25};

        U32  button_amount = 0;
        bool play          = false;
        bool pause         = false;
        bool step          = false;
        bool stop          = false;
        bool build         = false;
        bool build_and_run = false;

        if (state == State::Paused || state == State::Editing || state == State::Step) {
            ++button_amount;
            play = true;
        }
        if (state == State::Running) {
            ++button_amount;
            pause = true;
        }
        if (state == State::Paused || state == State::Step) {
            ++button_amount;
            step = true;
        }
        if (state == State::Paused || state == State::Running || state == State::Step) {
            ++button_amount;
            stop = true;
        }
        if (state == State::Editing) {
            ++button_amount;
            build = true;
        }
        if (state == State::Editing) {
            ++button_amount;
            build_and_run = true;
        }
        ImVec2 cursor_pos{m_ViewportSize.x * 0.5f - (button_amount * button_size.x + (button_amount - 1) * button_padding.x) / 2, button_padding.y};

        if (play) {
            ImGui::SetCursorPos(cursor_pos);
            cursor_pos.x += button_size.x + button_padding.x;
            if (ImGui::ImageButton("Play", resources.PlayIcon->GetRendererId(), button_size)) {
                if (state == State::Editing) {
                    editor.ActionRunScene();
                } else {
                    editor.ActionResumeScene();
                }
            }
        }
        if (pause) {
            ImGui::SetCursorPos(cursor_pos);
            cursor_pos.x += button_size.x + button_padding.x;
            if (ImGui::ImageButton("Pause", resources.PauseIcon->GetRendererId(), button_size)) {
                editor.ActionPauseScene();
            }
        }
        if (step) {
            ImGui::SetCursorPos(cursor_pos);
            cursor_pos.x += button_size.x + button_padding.x;
            if (ImGui::ImageButton("Step", resources.StepIcon->GetRendererId(), button_size)) {
                editor.ActionStepScene();
            }
        }
        if (stop) {
            ImGui::SetCursorPos(cursor_pos);
            cursor_pos.x += button_size.x + button_padding.x;
            if (ImGui::ImageButton("Stop", resources.StopIcon->GetRendererId(), button_size)) {
                editor.ActionStopScene();
            }
        }
        if (build) {
            ImGui::SetCursorPos(cursor_pos);
            cursor_pos.x += button_size.x + button_padding.x;
            if (ImGui::ImageButton("Build", resources.BuildIcon->GetRendererId(), button_size)) {
                editor.ActionBuild();
            }
        }
        if (build_and_run) {
            ImGui::SetCursorPos(cursor_pos);
            cursor_pos.x += button_size.x + button_padding.x;
            if (ImGui::ImageButton("BuildAndRun", resources.BuildAndRunIcon->GetRendererId(), button_size)) {
                editor.ActionBuildAndRun();
            }
        }
    }
    void ViewportPanel::SetFrameBuffer(Ref<FrameBuffer> buffer) {
        m_FrameBuffer = buffer;
    }
}  // namespace DummyEngine