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

    template <typename Component> void InspectorPanel::DrawComponentWidget(Entity entity) {
        if (m_Entity.Has<Component>()) {
            std::string header = ICON_MD_REPORT_PROBLEM "  " + DemangledName<Component>();
            if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SetCursorPosX(ImGuiUtils::Constants::DefaultLeftPadding);
                ImGui::Text("Widget for %s not yet implmented", DemangledName<Component>().c_str());
            }
        }
    }

    template <> void InspectorPanel::DrawComponentWidget<TransformComponent>(Entity entity) {
        if (m_Entity.Has<TransformComponent>()) {
            if (ImGui::CollapsingHeader(ICON_MD_OPEN_IN_FULL "  Transformation", ImGuiTreeNodeFlags_DefaultOpen)) {
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
    template <> void InspectorPanel::DrawComponentWidget<TagComponent>(Entity entity) {
        if (m_Entity.Has<TagComponent>()) {
            auto& component = m_Entity.Get<TagComponent>();
            if (ImGui::CollapsingHeader(ICON_MD_BADGE "  Tag", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Tag", component.tag);
                ImGui::Columns(1);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<ScriptComponent>(Entity entity) {
        if (m_Entity.Has<ScriptComponent>()) {
            auto& component = m_Entity.Get<ScriptComponent>();
            if (ImGui::CollapsingHeader(ICON_MD_DESCRIPTION "  Script", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Columns(2);
                if (component.Valid()) {
                    auto scene = component->GetScene();
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
                            case ScriptFieldType::Entity: ImGuiUtils::EditProperty(name.get(), field.Get<Entity>(), scene); break;
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
    template <> void InspectorPanel::DrawComponentWidget<FPSCamera>(Entity entity) {
        if (m_Entity.Has<FPSCamera>()) {
            if (ImGui::CollapsingHeader(ICON_MD_VIDEOCAM "  FPSCamera", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& camera = m_Entity.Get<FPSCamera>();
                ImGui::Columns(2);

                ImGuiUtils::EditProperty("Position", camera.m_Position);
                ImGuiUtils::EditProperty("NearPlane", camera.m_NearPlane);
                ImGuiUtils::EditProperty("FarPlane", camera.m_FarPlane);
                ImGuiUtils::EditProperty("FIeldOfView", camera.m_FOV);
                ImGui::Columns(1);
                if (ImGui::Button("Select as main camera", {ImGui::GetContentRegionAvail().x, 20})) {
                    m_Scene.lock()->SetCamera(entity);
                }
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<LightSource>(Entity entity) {
        if (m_Entity.Has<LightSource>()) {
            if (ImGui::CollapsingHeader(ICON_MD_LIGHTBULB "  LightSoruce", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto&       source        = m_Entity.Get<LightSource>();
                const char* light_types[] = {"Directional", "Point", "Spot"};
                ImGui::Columns(2);

                ImGui::SetCursorPosX(ImGuiUtils::Constants::DefaultLeftPadding);
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
    template <> void InspectorPanel::DrawComponentWidget<RenderMeshComponent>(Entity entity) {
        if (m_Entity.Has<RenderMeshComponent>()) {
            if (ImGui::CollapsingHeader(ICON_MD_TOKEN "  RenderMesh", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& meshes        = m_Entity.Get<RenderMeshComponent>().mesh_instance->GetMesh()->GetSubMeshes();
                auto& material      = meshes.front().material;
                float max_orm       = 1;
                float min_orm       = 0;
                float min_shininess = 0;
                float orm_speed     = 0.01;
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("AmbientColor", material.ambient, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("AlbedoColor", material.albedo_color, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("AmbientOcclusion", ImGuiDataType_Float, &material.orm.r, orm_speed, &min_orm, &max_orm);
                ImGuiUtils::EditProperty("Roughness", ImGuiDataType_Float, &material.orm.g, orm_speed, &min_orm, &max_orm);
                ImGuiUtils::EditProperty("Metallic", ImGuiDataType_Float, &material.orm.b, orm_speed, &min_orm, &max_orm);
                ImGuiUtils::EditProperty("Shininess", ImGuiDataType_Float, &material.shininess, 1.0f, &min_shininess, nullptr);
                ImGui::Columns(1);
                ImGui::Separator();
                int cnt = 0;
                for (auto& mesh : meshes) {
                    mesh.material.ambient      = material.ambient;
                    mesh.material.albedo_color = material.albedo_color;
                    mesh.material.orm          = material.orm;
                    mesh.material.shininess    = material.shininess;
                    std::string name           = StrCat("Mesh ", cnt);
                    if (ImGui::TreeNode(name.c_str())) {
                        auto& material   = mesh.material;
                        auto  albedo_map = (material.albedo_map ? material.albedo_map : Renderer::GetDefaultTexture());
                        auto  normal_map = (material.normal_map ? material.normal_map : Renderer::GetDefaultNormalTexture());
                        auto  orm_map    = (material.orm_map ? material.orm_map : Renderer::GetDefaultTexture());
                        ImGui::Separator();
                        ImGui::Columns(2);
                        ImGuiUtils::EditTexture("AlbedoMap", albedo_map);
                        ImGui::Separator();
                        ImGuiUtils::EditTexture("NormalMap", normal_map);
                        ImGui::Separator();
                        ImGuiUtils::EditTexture("OrmMap", orm_map);
                        ImGui::Columns(1);
                        ImGui::Separator();
                        ImGui::TreePop();
                    }
                    ++cnt;
                }
            }
        }
    }

    void InspectorPanel::OnImGui() {
        DE_PROFILE_SCOPE("InspectorPanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_INFO "  Inspector")) {
                auto scene = m_Scene.lock();
                if (scene && m_Entity.Valid()) {
                    DrawComponentWidget<TagComponent>(m_Entity);
                    DrawComponentWidget<ScriptComponent>(m_Entity);
                    DrawComponentWidget<TransformComponent>(m_Entity);
                    DrawComponentWidget<FPSCamera>(m_Entity);
                    DrawComponentWidget<LightSource>(m_Entity);
                    DrawComponentWidget<RenderMeshComponent>(m_Entity);
                    DrawComponentWidget<SkyBox>(m_Entity);
                    AddComponent();
                }
            }
            ImGui::End();
        }
    }
    void InspectorPanel::AddComponent() {
        if (ImGui::Button(ICON_MD_ADD "Add Component", {ImGui::GetContentRegionAvail().x, 30})) {
            ImGui::OpenPopup("AddComponentPopup");
        }
        if (ImGui::BeginPopup("AddComponentPopup")) {
            if (!m_Entity.Has<TransformComponent>() && ImGui::Selectable(ICON_MD_OPEN_IN_FULL "Transform")) {
                m_Entity.Add<TransformComponent>();
            }
            if (!m_Entity.Has<LightSource>() && ImGui::Selectable(ICON_MD_LIGHTBULB "LightSource")) {
                m_Entity.Add<LightSource>();
            }
            if (!m_Entity.Has<FPSCamera>() && ImGui::Selectable(ICON_MD_VIDEOCAM "Camera")) {
                m_Entity.Add<FPSCamera>();
            }
            ImGui::EndPopup();
        }
    }
    void InspectorPanel::SetActiveEntity(Entity entity) {
        m_Entity = entity;
    }
    void InspectorPanel::SetScene(WeakRef<Scene> scene) {
        m_Scene = scene;
    }

}  // namespace DE