//
// Created by balanda on 9/29/2023.
//

#include "ConsoleLayer.hpp"

#include "DummyEngine/Core/Application/KeyCodes.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEditor/DummyEngineInclude.h"

void DE::ConsoleLayer::OnUpdate(float dt) {
    if (Input::KeyPressed(Key::I)) {
        m_Show = !m_Show;
    }
}

void DE::ConsoleLayer::OnImGuiRender() {
    if (m_Show) {
        if (ImGui::Begin("Console")) {
            if (m_historyPosition >= Console::GetCmdHistory().size()) {
                m_Command         = "";
                m_historyPosition = Console::GetCmdHistory().size() - 1;
            }
            if (m_historyPosition < 0) {
                m_historyPosition = Console::GetCmdHistory().size() - 1;
            }
            if (Input::KeyReleased(Key::Up)) {
                LOG_DEBUG("ConsoleLayer", "CmdHistoryLength = ", Console::GetCmdHistory().size(), ", Position = ", m_historyPosition, ", cmd=", Console::GetCmdHistory()[m_historyPosition]);
                m_Command = Console::GetCmdHistory()[m_historyPosition];
                m_historyPosition--;
               // ImGui::ClearActiveID();
            }
            if (Input::KeyReleased(Key::Down)) {
                m_Command = Console::GetCmdHistory()[m_historyPosition];
                m_historyPosition++;
               // ImGui::ClearActiveID();
            }


            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputTextWithHint("##", "command", &m_Command, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AlwaysOverwrite)) {
                m_historyPosition = Console::GetCmdHistory().size() - 1;
                Console::ExecuteCommand(m_Command);
                m_ScrollToBottom = true;
                m_Command = "";
            }
            if (ImGui::IsItemFocused()) {
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
                ImGui::BeginTooltip();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
                const auto& tips = Console::GetHints(m_Command);
                for (const auto& tip: tips) {
                    ImGui::TextUnformatted(tip.c_str());
                }
                ImGui::PopStyleVar();
                ImGui::EndTooltip();
            }

            ImGui::Separator();

            ImGui::BeginChild("log list");
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
            for (const auto& line: Console::GetLogHistory()) {
                ImGui::TextUnformatted(line.data());
            }
            if (m_ScrollToBottom) {
                ImGui::SetScrollHereY();
                m_ScrollToBottom = false;
            }
            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
        ImGui::End();
    }
}

void DE::ConsoleLayer::OnAttach() {
}
void DE::ConsoleLayer::OnDetach() {
}