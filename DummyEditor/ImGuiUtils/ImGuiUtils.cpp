#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    namespace ImGuiUtils {
        void ClampRoundValue(Vec3& vec, float min, float max) {
            if (vec.x < min) vec.x = max;
            if (vec.x > max) vec.x = min;
            if (vec.y < min) vec.y = max;
            if (vec.y > max) vec.y = min;
            if (vec.z < min) vec.z = max;
            if (vec.z > max) vec.z = min;
        }
        void ClampRoundValue(Vec4& vec, float min, float max) {
            if (vec.x < min) vec.x = max;
            if (vec.x > max) vec.x = min;
            if (vec.y < min) vec.y = max;
            if (vec.y > max) vec.y = min;
            if (vec.z < min) vec.z = max;
            if (vec.z > max) vec.z = min;
            if (vec.w < min) vec.z = max;
            if (vec.w > max) vec.z = min;
        }

        void EditProperty(std::string name, ImGuiDataType type, void* value, PropertyType property_type) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(Constants::DefaultRightPadding);
            name.insert(name.begin(), 2, '#');
            ImGui::DragScalar(name.c_str(), type, value);
            ImGui::NextColumn();
        }
        void EditProperty(const std::string& name, float& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_Float, &value);
        }
        void EditProperty(const std::string& name, double& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_Double, &value);
        }
        void EditProperty(const std::string& name, U8& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_U8, &value);
        }
        void EditProperty(const std::string& name, S8& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_S8, &value);
        }
        void EditProperty(const std::string& name, U16& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_U16, &value);
        }
        void EditProperty(const std::string& name, S16& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_S16, &value);
        }
        void EditProperty(const std::string& name, U32& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_U32, &value);
        }
        void EditProperty(const std::string& name, S32& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_S32, &value);
        }
        void EditProperty(const std::string& name, U64& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_U64, &value);
        }
        void EditProperty(const std::string& name, S64& value, PropertyType property_type) {
            EditProperty(name, ImGuiDataType_S64, &value);
        }
        void EditProperty(const std::string& name, bool& value, PropertyType property_type) {
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(-1);
            ImGui::Checkbox(name.c_str(), &value);
            ImGui::NextColumn();
        }
        void EditProperty(std::string name, std::string& value, PropertyType property_type) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(Constants::DefaultRightPadding);
            name.insert(name.begin(), 2, '#');
            ImGui::InputText(name.c_str(), &value);
            ImGui::NextColumn();
        }
        void EditProperty(std::string name, Vec2& vec, PropertyType property_type) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(Constants::DefaultRightPadding);
            name.insert(name.begin(), 2, '#');
            ImGui::DragFloat2(name.c_str(), &vec.x);
            ImGui::NextColumn();
        }
        void EditProperty(std::string name, Vec3& vec, PropertyType property_type) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            name.insert(name.begin(), 2, '#');
            switch (property_type) {
                case PropertyType::Color:
                    ImGui::SetNextItemWidth(Constants::ColorPading);
                    ImGui::ColorEdit3(name.c_str(), &vec.x, ImGuiColorEditFlags_NoLabel);
                    break;
                default:
                    ImGui::SetNextItemWidth(Constants::DefaultRightPadding);
                    ImGui::DragFloat3(name.c_str(), &vec.x);
                    break;
            }
            ImGui::NextColumn();
        }
        void EditProperty(std::string name, Vec4& vec, PropertyType property_type) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            name.insert(name.begin(), 2, '#');
            switch (property_type) {
                case PropertyType::Color:
                    ImGui::SetNextItemWidth(Constants::ColorPading);
                    ImGui::ColorEdit4(name.c_str(), &vec.x, ImGuiColorEditFlags_NoLabel);
                    break;
                default:
                    ImGui::SetNextItemWidth(Constants::DefaultRightPadding);
                    ImGui::DragFloat4(name.c_str(), &vec.x);
                    break;
            }
            ImGui::NextColumn();
        }
        void EditProperty(const std::string& name, ImVec4& vec, PropertyType property_type) {
            EditProperty(name, *(Vec4*)&vec, property_type);
        }

    }  // namespace ImGuiUtils
}  // namespace DE