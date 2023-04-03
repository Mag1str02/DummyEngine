#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    void ImGuiItem::Enable() {
        m_Controller = true;
    }
    void ImGuiItem::Disable() {
        m_Controller = false;
    }
    bool& ImGuiItem::GetController() {
        return m_Controller;
    }
    const std::string& ImGuiItem::GetName() const {
        return m_Name;
    }
    namespace ImGuiUtils {
        void EditProperty(const std::string& name, Vec2& vec, PropertyType type) {}
        void EditProperty(const std::string& name, Vec3& vec, PropertyType type) {
            ImGui::Text("%s", name.c_str());
            ImGui::NextColumn();
            switch (type) {
                case PropertyType::Color:
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3(name.c_str(), &vec.x, ImGuiColorEditFlags_NoLabel);
                    break;
                default:
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3(name.c_str(), &vec.x);
                    break;
            }
            ImGui::NextColumn();
        }
        void EditProperty(const std::string& name, Vec4& vec, PropertyType type) {
            ImGui::Text("%s", name.c_str());
            ImGui::NextColumn();
            switch (type) {
                case PropertyType::Color:
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit4(name.c_str(), &vec.x, ImGuiColorEditFlags_NoLabel);
                    break;
                default:
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat4(name.c_str(), &vec.x);
                    break;
            }
            ImGui::NextColumn();
        }
        void EditProperty(const std::string& name, ImVec4& vec, PropertyType type) {
            EditProperty(name, *(Vec4*)&vec, type);
        }

    }  // namespace ImGuiUtils
}  // namespace DE