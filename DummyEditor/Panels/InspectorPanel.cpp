#include "DummyEditor/Panels/InspectorPanel.h"

namespace DE {
    void ClampRoundValue(Vec3& vec, float min, float max) {
        if (vec.x < min) vec.x = max;
        if (vec.x > max) vec.x = min;
        if (vec.y < min) vec.y = max;
        if (vec.y > max) vec.y = min;
        if (vec.z < min) vec.z = max;
        if (vec.z > max) vec.z = min;
    }

    ImGuiDataType_ ScriptFieldTypeToImGuiType(ScriptFieldType type) {
        switch (type) {
            case ScriptFieldType::Double: return ImGuiDataType_Double;
            case ScriptFieldType::Float: return ImGuiDataType_Float;
            case ScriptFieldType::S32: return ImGuiDataType_S32;
            case ScriptFieldType::U32: return ImGuiDataType_U32;
            case ScriptFieldType::S64: return ImGuiDataType_S64;
            case ScriptFieldType::U64: return ImGuiDataType_U64;
            default: return ImGuiDataType_COUNT;
        }
    }
    void InspectorPanel::View() {
        DE_PROFILE_SCOPE("InspectorPanel View");

        float sensitivity = 0.1;

        if (m_Entity.Valid()) {
            if (m_Entity.Has<ScriptComponent>()) {
                auto& component = m_Entity.Get<ScriptComponent>();
                if (ImGui::CollapsingHeader("Script")) {
                    ImGui::Text("UUID: %s", component.ID().Hex().c_str());
                    if (component.Valid()) {
                        for (auto [name, field] : *component) {
                            if (field.GetType() != ScriptFieldType::None) {
                                ImGui::Text("%s", name.get().c_str());
                                ImGui::SameLine(100);
                                ImGui::SetNextItemWidth(-1);
                            }

                            switch (field.GetType()) {
                                case ScriptFieldType::Double:
                                case ScriptFieldType::Float:
                                case ScriptFieldType::S32:
                                case ScriptFieldType::U32:
                                case ScriptFieldType::S64:
                                case ScriptFieldType::U64:
                                    ImGui::DragScalar(("##" + name.get()).c_str(), ScriptFieldTypeToImGuiType(field.GetType()), field.Get());
                                    break;
                                case ScriptFieldType::Bool: ImGui::Checkbox(("##" + name.get()).c_str(), &field.Get<bool>()); break;
                                case ScriptFieldType::String: ImGui::InputText(("##" + name.get()).c_str(), &field.Get<std::string>()); break;
                                case ScriptFieldType::Vec2: ImGui::DragFloat2(("##" + name.get()).c_str(), &field.Get<Vec2>()[0]); break;
                                case ScriptFieldType::Vec3: ImGui::DragFloat3(("##" + name.get()).c_str(), &field.Get<Vec3>()[0]); break;
                                case ScriptFieldType::Vec4: ImGui::DragFloat4(("##" + name.get()).c_str(), &field.Get<Vec4>()[0]); break;
                                default: break;
                            }
                        }
                    }
                }
            }
            if (m_Entity.Has<TagComponent>()) {
                auto& component = m_Entity.Get<TagComponent>();
                if (ImGui::CollapsingHeader("Tag")) {
                    ImGui::Text("Tag");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputText("", &component.Get());
                }
            }
            if (m_Entity.Has<IdComponent>()) {
                auto& component = m_Entity.Get<IdComponent>();
                if (ImGui::CollapsingHeader("UUID")) {
                    ImGui::Text("%s", component.Hex().c_str());
                }
            }
            if (m_Entity.Has<TransformComponent>()) {
                if (ImGui::CollapsingHeader("Transformation")) {
                    auto& transform = m_Entity.Get<TransformComponent>();

                    ImGui::Text("Translation");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Translation", &(transform.translation.x), sensitivity, 0, 0);

                    ImGui::Text("Scale");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Scale", &(transform.scale.x), sensitivity, 0, 0);

                    ImGui::Text("Rotation");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Rotation", &(transform.rotation.x), sensitivity, 0, 0);

                    ClampRoundValue(transform.rotation, 0, 360);
                }
            }
            if (m_Entity.Has<RenderMeshComponent>()) {
                if (ImGui::CollapsingHeader("Render Mesh")) {
                    auto& render_mesh = m_Entity.Get<RenderMeshComponent>();
                    ImGui::Text("%s", ("Mesh UUID: " + render_mesh.id.Hex()).c_str());
                }
            }
            if (m_Entity.Has<FPSCamera>()) {
                if (ImGui::CollapsingHeader("FPSCamera")) {
                    auto& camera = m_Entity.Get<FPSCamera>();

                    ImGui::Text("Position");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Position", &(camera.m_Position.x), sensitivity, 0, 0);

                    ImGui::Text("Near Plane");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat("##Near Plane", &(camera.m_NearPlane), sensitivity, 0, 0);

                    ImGui::Text("Far Plane");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat("##Far Plane", &(camera.m_FarPlane), sensitivity, 0, 0);

                    ImGui::Text("FOV");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat("##FOV", &(camera.m_FOV), sensitivity, 0, 360);
                }
            }
            if (m_Entity.Has<LightSource>()) {
                if (ImGui::CollapsingHeader("Light Soruce")) {
                    auto&       source        = m_Entity.Get<LightSource>();
                    const char* light_types[] = {"Directional", "Point", "Spot"};

                    ImGui::Text("Type");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-24);
                    ImGui::Combo("##Light Type", (int*)&source.type, light_types, IM_ARRAYSIZE(light_types));
                    ImGui::Separator();

                    ImGui::Text("Ambient");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3("##Ambient", &(source.ambient.x));

                    ImGui::Text("Diffuse");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3("##Diffuse", &(source.diffuse.x));

                    ImGui::Text("Specular");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3("##Specular", &(source.specular.x));

                    if (source.type == LightSourceType::Point || source.type == LightSourceType::Spot) {
                        ImGui::Text("CLQ");
                        ImGui::SameLine(100);
                        ImGui::SetNextItemWidth(-24);
                        ImGui::DragFloat3("##CLQ", &(source.clq.x), sensitivity, 0, 1);
                    }
                    if (source.type == LightSourceType::Spot || source.type == LightSourceType::Direction) {
                        ImGui::Text("Direction");
                        ImGui::SameLine(100);
                        ImGui::SetNextItemWidth(-24);
                        ImGui::DragFloat3("##Direction", &(source.direction.x), sensitivity, 0, 1);
                    }
                    if (source.type == LightSourceType::Spot || source.type == LightSourceType::Point) {
                        ImGui::Text("Position");
                        ImGui::SameLine(100);
                        ImGui::SetNextItemWidth(-24);
                        ImGui::DragFloat3("##Position", &(source.position.x), sensitivity);
                    }
                    if (source.type == LightSourceType::Spot) {
                        ImGui::Text("Inner Cone");
                        ImGui::SameLine(100);
                        ImGui::SetNextItemWidth(-24);
                        ImGui::DragFloat("##InnerCone", &(source.inner_cone_cos), sensitivity, 0, 1);

                        ImGui::Text("Outer Cone");
                        ImGui::SameLine(100);
                        ImGui::SetNextItemWidth(-24);
                        ImGui::DragFloat("##OuterCone", &(source.outer_cone_cos), sensitivity, 0, 1);
                    }
                }
            }
        }
    }
    void InspectorPanel::SetActiveEntity(Entity entity) {
        m_Entity = entity;
    }

}  // namespace DE