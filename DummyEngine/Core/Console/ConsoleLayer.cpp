#include "ConsoleLayer.hpp"

#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Application/KeyCodes.h"
#include "DummyEngine/Core/Console/Console.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace DummyEngine {

    void ConsoleLayer::OnUpdate(float) {
        if (Input::KeyPressed(Key::I) && Input::KeyDown(Key::LeftShift)) {
            if (!show_) {
                just_opened_ = true;
            }
            show_ = !show_;
        }
    }

    void ConsoleLayer::OnImGuiRender() {
        if (show_) {
            if (ImGui::Begin("Console")) {
                if (history_position_ >= static_cast<S32>(Console::GetCmdHistory().size())) {
                    command_          = "";
                    history_position_ = Console::GetCmdHistory().size() - 1;
                }
                if (history_position_ < 0) {
                    history_position_ = Console::GetCmdHistory().size() - 1;
                }
                if (Input::KeyReleased(Key::Up)) {
                    LOG_DEBUG("CmdHistoryLength = {}, Position = {}, cmd = {}",
                              Console::GetCmdHistory().size(),
                              history_position_,
                              Console::GetCmdHistory()[history_position_]);
                    command_ = Console::GetCmdHistory()[history_position_];
                    history_position_--;
                    just_opened_ = true;
                }
                if (Input::KeyReleased(Key::Down)) {
                    command_ = Console::GetCmdHistory()[history_position_];
                    history_position_++;
                    just_opened_ = true;
                }
                if (just_opened_) {
                    just_opened_ = false;
                    ImGui::SetKeyboardFocusHere();
                }
                if (ImGui::InputTextWithHint(
                        "##", "command", &command_, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AlwaysOverwrite)) {
                    history_position_ = Console::GetCmdHistory().size() - 1;
                    Console::ExecuteCommand(command_);
                    scroll_to_bottom_ = true;
                    command_          = "";
                    just_opened_      = true;
                }
                if (ImGui::IsItemFocused()) {
                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
                    if (ImGui::BeginTooltip()) {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
                        const auto& tips = Console::GetHints(command_);
                        if (!tips.empty()) {
                            for (const auto& tip : tips) {
                                ImGui::TextUnformatted(tip.c_str());
                            }
                        } else {
                            ImGui::TextUnformatted("No commands found");
                        }

                        ImGui::PopStyleVar();
                        ImGui::EndTooltip();
                    }
                }

                ImGui::Separator();

                ImGui::BeginChild("log list");
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
                for (const auto& line : Console::GetLogHistory()) {
                    ImGui::TextUnformatted(line.data());
                }
                if (scroll_to_bottom_) {
                    ImGui::SetScrollHereY();
                    scroll_to_bottom_ = false;
                }
                ImGui::PopStyleVar();
                ImGui::EndChild();
            }
            ImGui::End();
        }
    }

    void ConsoleLayer::OnAttach() {}
    void ConsoleLayer::OnDetach() {}

}  // namespace DummyEngine
