#pragma once

#include "DummyEditor/DummyEngineInclude.h"

namespace DE {
    class ImGuiItem {
    public:
        ImGuiItem(const std::string& name) : m_Name(name) {}
        virtual ~ImGuiItem() = default;

        virtual void       OnImGui() = 0;
        void               Enable();
        void               Disable();
        bool&              GetController();
        const std::string& GetName() const;

    protected:
        bool        m_Controller = true;
        std::string m_Name       = "UnnamedItem";
    };
    constexpr ImVec4 U32ToColor(uint32_t color) {
        return {float((color & 0xFF0000) >> 16) / 255,
                float((color & 0xFF00) >> 8) / 255,
                float(color & 0xFF) / 255,
                float((color & 0xFF000000) >> 24) / 255};
    }

}  // namespace DE
