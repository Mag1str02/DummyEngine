#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    class ImGuiPanel : public ImGuiItem {
    public:
        ImGuiPanel(const std::string& name) : ImGuiItem(name) {}
        virtual ~ImGuiPanel() = default;
        virtual void View()   = 0;
        virtual void PushStyle() {}
        virtual void PopStyle() {}

        virtual void OnImGui() final;
    };
}  // namespace DE
