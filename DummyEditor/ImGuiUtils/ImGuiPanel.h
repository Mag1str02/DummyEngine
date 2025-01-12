#pragma once

#include <string>

namespace DummyEngine {

    class ImGuiPanel {
    public:
        explicit ImGuiPanel(const std::string& name) : m_Name(name) {}
        virtual ~ImGuiPanel() = default;

        virtual void       OnImGui() = 0;
        void               Enable();
        void               Disable();
        bool&              GetController();
        const std::string& GetName() const;

    protected:
        bool        m_Controller = true;
        std::string m_Name       = "UnnamedPanel";
    };
}  // namespace DummyEngine
