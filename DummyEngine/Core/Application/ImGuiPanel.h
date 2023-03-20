#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    class ImGuiPanel {
    public:
        ImGuiPanel(const std::string& name) : m_Name(name) {}
        virtual ~ImGuiPanel(){};
        virtual void View() = 0;
        virtual void PushStyle() {}
        virtual void PopStyle() {}

        void OnImGuiRender();
        void SetController(bool& enabled) { m_Enabled = &enabled; }

        bool*              GetController() { return m_Enabled; }
        const std::string& GetName() const { return m_Name; }

    private:
        bool*       m_Enabled = nullptr;
        std::string m_Name;
    };
}  // namespace DE
