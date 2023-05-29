#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

namespace DE {
    namespace ImGuiUtils {
        ScopedID::ScopedID(int cnt) {
            ImGui::PushID(cnt);
        }
        ScopedID::ScopedID(const char* str) {
            ImGui::PushID(str);
        }
        ScopedID::ScopedID(const std::string& str) {
            ImGui::PushID(str.c_str());
        }
        ScopedID::ScopedID(const void* ptr) {
            ImGui::PushID(ptr);
        }
        ScopedID::~ScopedID() {
            ImGui::PopID();
        }

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

        void EditProperty(std::string name, ImGuiDataType type, void* value, float speed, const void* min, const void* max) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(Constants::DefaultRightPadding);
            name.insert(name.begin(), 2, '#');
            ImGui::DragScalar(name.c_str(), type, value, speed, min, max);
            ImGui::NextColumn();
        }
        void EditProperty(const std::string& name, float& value) {
            EditProperty(name, ImGuiDataType_Float, &value);
        }
        void EditProperty(const std::string& name, double& value) {
            EditProperty(name, ImGuiDataType_Double, &value);
        }
        void EditProperty(const std::string& name, U8& value) {
            EditProperty(name, ImGuiDataType_U8, &value);
        }
        void EditProperty(const std::string& name, S8& value) {
            EditProperty(name, ImGuiDataType_S8, &value);
        }
        void EditProperty(const std::string& name, U16& value) {
            EditProperty(name, ImGuiDataType_U16, &value);
        }
        void EditProperty(const std::string& name, S16& value) {
            EditProperty(name, ImGuiDataType_S16, &value);
        }
        void EditProperty(const std::string& name, U32& value) {
            EditProperty(name, ImGuiDataType_U32, &value);
        }
        void EditProperty(const std::string& name, S32& value) {
            EditProperty(name, ImGuiDataType_S32, &value);
        }
        void EditProperty(const std::string& name, U64& value) {
            EditProperty(name, ImGuiDataType_U64, &value);
        }
        void EditProperty(const std::string& name, S64& value) {
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
        void EditProperty(const std::string& name, Entity& entity, Ref<Scene> scene) {
            ImGui::SetCursorPosX(Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            std::string preview = "Invalid entity";
            if (entity.Valid()) {
                preview = entity.Get<TagComponent>();
            }
            int cnt = 0;
            ImGui::SetNextItemWidth(Constants::ColorPading);
            if (ImGui::BeginCombo("##EntitySelect", preview.c_str())) {
                for (auto e : scene->View<TagComponent, IDComponent>()) {
                    ImGuiUtils::ScopedID id(cnt++);
                    if (ImGui::Selectable(e.Get<TagComponent>().tag.c_str(), entity == e)) {
                        entity = e;
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::NextColumn();
        }
        void EditProperty(Material& mat) {
            const char* mat_types[]   = {"Unknown", "Phong", "PBR"};
            float       max_orm       = 1;
            float       min_orm       = 0;
            float       min_shininess = 0;
            float       orm_speed     = 0.01;

            ImGui::Separator();
            ImGui::SetCursorPosX(ImGuiUtils::Constants::DefaultLeftPadding);
            ImGui::Text("Type");
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(-1);
            ImGui::Combo("##Material Type", (int*)&mat.type, mat_types, IM_ARRAYSIZE(mat_types));
            ImGui::NextColumn();
            ImGui::Separator();
            switch (mat.type) {
                case MaterialType::PBR: {
                    auto albedo_map = (mat.albedo_map ? mat.albedo_map : Renderer::GetTexture(Renderer::Textures::White));
                    auto normal_map = (mat.normal_map ? mat.normal_map : Renderer::GetTexture(Renderer::Textures::Normal));
                    auto orm_map    = (mat.orm_map ? mat.orm_map : Renderer::GetTexture(Renderer::Textures::White));

                    ImGuiUtils::EditProperty("AmbientColor", mat.ambient, ImGuiUtils::PropertyType::Color);
                    ImGuiUtils::EditProperty("AlbedoColor", mat.albedo, ImGuiUtils::PropertyType::Color);
                    ImGuiUtils::EditProperty("AmbientOcclusion", ImGuiDataType_Float, &mat.orm.r, orm_speed, &min_orm, &max_orm);
                    ImGuiUtils::EditProperty("Roughness", ImGuiDataType_Float, &mat.orm.g, orm_speed, &min_orm, &max_orm);
                    ImGuiUtils::EditProperty("Metallic", ImGuiDataType_Float, &mat.orm.b, orm_speed, &min_orm, &max_orm);
                    ImGui::Separator();
                    ImGuiUtils::EditTexture("AlbedoMap", albedo_map);
                    ImGui::Separator();
                    ImGuiUtils::EditTexture("OrmMap", orm_map);
                    ImGui::Separator();
                    ImGuiUtils::EditTexture("NormalMap", normal_map);
                    ImGui::Separator();
                    break;
                }
                case MaterialType::Phong: {
                    auto normal_map   = (mat.normal_map ? mat.normal_map : Renderer::GetTexture(Renderer::Textures::Normal));
                    auto diffuse_map  = (mat.diffuse_map ? mat.diffuse_map : Renderer::GetTexture(Renderer::Textures::White));
                    auto specular_map = (mat.specular_map ? mat.specular_map : Renderer::GetTexture(Renderer::Textures::White));

                    ImGuiUtils::EditProperty("AmbientColor", mat.ambient, ImGuiUtils::PropertyType::Color);
                    ImGuiUtils::EditProperty("DiffuseColor", mat.diffuse, ImGuiUtils::PropertyType::Color);
                    ImGuiUtils::EditProperty("Specular", mat.specular, ImGuiUtils::PropertyType::Color);
                    ImGuiUtils::EditProperty("Shininess", ImGuiDataType_Float, &mat.shininess, orm_speed, &min_shininess, nullptr);
                    ImGui::Separator();
                    ImGuiUtils::EditTexture("DiffuseMap", diffuse_map);
                    ImGui::Separator();
                    ImGuiUtils::EditTexture("SpecularMap", specular_map);
                    ImGui::Separator();
                    ImGuiUtils::EditTexture("NormalMap", normal_map);
                    ImGui::Separator();
                    break;
                }

                default: break;
            }
        }
        void EditTexture(const std::string& name, Ref<Texture> texture, const ImVec2 tex_size) {
            ImGui::SetCursorPosX(ImGuiUtils::Constants::DefaultLeftPadding);
            ImGui::TextUnformatted(name.c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(ImGuiUtils::Constants::DefaultRightPadding);
            ImGui::Image(reinterpret_cast<void*>(texture->RendererId()), tex_size);
            ImGui::NextColumn();
        }
    }  // namespace ImGuiUtils
}  // namespace DE