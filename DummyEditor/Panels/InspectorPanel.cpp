#include "DummyEditor/Panels/InspectorPanel.h"

namespace DE {

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

    template <typename Component> void InspectorPanel::DrawComponentWidget(Ref<Scene> scene, Entity entity) {
        if (m_Entity.Has<Component>()) {
            std::string name = DemangledName<Component>();
            if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Widget for %s not yet implmented", name.c_str());
            }
        }
    }

    template <> void InspectorPanel::DrawComponentWidget<TransformComponent>(Ref<Scene> scene, Entity entity) {
        if (m_Entity.Has<TransformComponent>()) {
            if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& transform = m_Entity.Get<TransformComponent>();

                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Translation", transform.translation);
                ImGuiUtils::EditProperty("Scale", transform.scale);
                ImGuiUtils::EditProperty("Rotation", transform.rotation);
                ImGuiUtils::ClampRoundValue(transform.rotation, 0, 360);
                ImGui::Columns(1);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<TagComponent>(Ref<Scene> scene, Entity entity) {
        if (m_Entity.Has<TagComponent>()) {
            // TODO: Find proper library function or write own
            auto&      component = m_Entity.Get<TagComponent>();
            static int swap      = 2;
            if (ImGui::CollapsingHeader("Tag", ImGuiTreeNodeFlags_DefaultOpen)) {
                static std::string current_name = "";
                if (swap == 2) {
                    current_name = component.Get();
                }
                if (swap == 1) {
                    m_Entity.Remove<TagComponent>();
                    m_Entity.AddComponent<TagComponent>(scene->GenAvilableEntityName(current_name));
                }
                swap = 0;
                ImGui::Columns(2);
                ImGui::Text("Tag");
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);

                ImGui::InputText("##", &current_name, ImGuiInputTextFlags_CharsNoBlank);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    swap = 1;
                }
                if (ImGui::IsItemActivated()) {
                    swap = 2;
                }
                ImGui::NextColumn();
                ImGui::Columns(1);
            } else {
                swap = 2;
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<ScriptComponent>(Ref<Scene> scene, Entity entity) {
        if (m_Entity.Has<ScriptComponent>()) {
            auto& component = m_Entity.Get<ScriptComponent>();
            if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Columns(2);
                if (component.Loaded()) {
                    for (auto [name, field] : *component) {
                        switch (field.GetType()) {
                            case ScriptFieldType::Double: ImGuiUtils::EditProperty(name.get(), field.Get<double>()); break;
                            case ScriptFieldType::Float: ImGuiUtils::EditProperty(name.get(), field.Get<float>()); break;
                            case ScriptFieldType::S8: ImGuiUtils::EditProperty(name.get(), field.Get<S8>()); break;
                            case ScriptFieldType::U8: ImGuiUtils::EditProperty(name.get(), field.Get<U8>()); break;
                            case ScriptFieldType::S16: ImGuiUtils::EditProperty(name.get(), field.Get<S16>()); break;
                            case ScriptFieldType::U16: ImGuiUtils::EditProperty(name.get(), field.Get<U16>()); break;
                            case ScriptFieldType::S32: ImGuiUtils::EditProperty(name.get(), field.Get<S32>()); break;
                            case ScriptFieldType::U32: ImGuiUtils::EditProperty(name.get(), field.Get<U32>()); break;
                            case ScriptFieldType::S64: ImGuiUtils::EditProperty(name.get(), field.Get<S64>()); break;
                            case ScriptFieldType::U64: ImGuiUtils::EditProperty(name.get(), field.Get<U64>()); break;
                            case ScriptFieldType::Bool: ImGuiUtils::EditProperty(name.get(), field.Get<bool>()); break;
                            case ScriptFieldType::String: ImGuiUtils::EditProperty(name.get(), field.Get<std::string>()); break;
                            case ScriptFieldType::Vec2: ImGuiUtils::EditProperty(name.get(), field.Get<Vec2>()); break;
                            case ScriptFieldType::Vec3: ImGuiUtils::EditProperty(name.get(), field.Get<Vec3>()); break;
                            case ScriptFieldType::Vec4: ImGuiUtils::EditProperty(name.get(), field.Get<Vec4>()); break;
                            default: break;
                        }
                    }
                } else {
                    // TODO: Load script component
                    ImGui::TextUnformatted("Script not loaded yet");
                }
                ImGui::Columns(1);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<FPSCamera>(Ref<Scene> scene, Entity entity) {
        if (m_Entity.Has<FPSCamera>()) {
            if (ImGui::CollapsingHeader("FPSCamera", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& camera = m_Entity.Get<FPSCamera>();

                ImGui::Text("Position");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(-1);
                ImGui::DragFloat3("##Position", &(camera.m_Position.x), m_SliderSensitivity, 0, 0);

                ImGui::Text("Near Plane");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(-1);
                ImGui::DragFloat("##Near Plane", &(camera.m_NearPlane), m_SliderSensitivity, 0, 0);

                ImGui::Text("Far Plane");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(-1);
                ImGui::DragFloat("##Far Plane", &(camera.m_FarPlane), m_SliderSensitivity, 0, 0);

                ImGui::Text("FOV");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(-1);
                ImGui::DragFloat("##FOV", &(camera.m_FOV), m_SliderSensitivity, 0, 360);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<LightSource>(Ref<Scene> scene, Entity entity) {
        if (m_Entity.Has<LightSource>()) {
            if (ImGui::CollapsingHeader("Light Soruce", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto&       source        = m_Entity.Get<LightSource>();
                const char* light_types[] = {"Directional", "Point", "Spot"};
                ImGui::Columns(2);

                ImGui::Text("Type");
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);
                ImGui::Combo("##Light Type", (int*)&source.type, light_types, IM_ARRAYSIZE(light_types));
                ImGui::NextColumn();
                ImGui::Separator();

                ImGuiUtils::EditProperty("Ambient", source.ambient, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Diffuse", source.diffuse, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Specular", source.specular, ImGuiUtils::PropertyType::Color);

                if (source.type == LightSourceType::Point || source.type == LightSourceType::Spot) {
                    ImGuiUtils::EditProperty("CLQ", source.clq);
                }
                if (source.type == LightSourceType::Spot || source.type == LightSourceType::Direction) {
                    ImGuiUtils::EditProperty("Direction", source.direction);
                }
                if (source.type == LightSourceType::Spot || source.type == LightSourceType::Point) {
                    ImGuiUtils::EditProperty("Position", source.position);
                }
                if (source.type == LightSourceType::Spot) {
                    ImGuiUtils::EditProperty("Inner Cone", source.inner_cone_cos);
                    ImGuiUtils::EditProperty("Outer Cone", source.outer_cone_cos);
                }
                ImGui::Columns(1);
            }
        }
    }

    void InspectorPanel::View() {
        DE_PROFILE_SCOPE("InspectorPanel View");
        auto scene = m_Scene.lock();
        if (!scene) {
            return;
        }

        if (m_Entity.Valid()) {
            DrawComponentWidget<TagComponent>(scene, m_Entity);
            DrawComponentWidget<ScriptComponent>(scene, m_Entity);
            DrawComponentWidget<TransformComponent>(scene, m_Entity);
            DrawComponentWidget<FPSCamera>(scene, m_Entity);
            DrawComponentWidget<LightSource>(scene, m_Entity);
            DrawComponentWidget<RenderMeshComponent>(scene, m_Entity);
        }
    }
    void InspectorPanel::SetActiveEntity(Entity entity) {
        m_Entity = entity;
    }
    void InspectorPanel::SetScene(WeakRef<Scene> scene) {
        m_Scene = scene;
    }

}  // namespace DE