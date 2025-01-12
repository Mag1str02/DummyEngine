#include "InspectorPanel.h"

#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Physics/PhysicsComponent.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Utils/Debug/Demangler.h"  // IWYU pragma: keep
#include "DummyEngine/Utils/Debug/Profiler.h"

#include <imgui.h>

namespace DummyEngine {

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

    template <typename Component> void InspectorPanel::DrawComponentWidget() {
        if (entity_.Has<Component>()) {
            std::string header = ICON_MD_REPORT_PROBLEM "  " + DemangledName<Component>();
            if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SetCursorPosX(ImGuiUtils::Constants::kDefaultLeftPadding);
                ImGui::Text("Widget for %s not yet implmented", DemangledName<Component>().c_str());
            }
        }
    }

    template <> void InspectorPanel::DrawComponentWidget<TransformComponent>() {
        if (entity_.Has<TransformComponent>()) {
            if (ImGui::CollapsingHeader(ICON_MD_OPEN_IN_FULL "  Transformation", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& transform = entity_.Get<TransformComponent>();
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Translation", transform.Translation);
                ImGuiUtils::EditProperty("Scale", transform.Scale);
                ImGuiUtils::EditProperty("Rotation", transform.Rotation);
                ImGui::Columns(1);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<TagComponent>() {
        if (entity_.Has<TagComponent>()) {
            auto& component = entity_.Get<TagComponent>();
            if (ImGui::CollapsingHeader(ICON_MD_BADGE "  Tag", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Tag", component.Tag);
                ImGui::Columns(1);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<ScriptComponent>() {
        if (entity_.Has<ScriptComponent>()) {
            auto& component = entity_.Get<ScriptComponent>();
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
    template <> void InspectorPanel::DrawComponentWidget<FPSCamera>() {
        if (entity_.Has<FPSCamera>()) {
            if (ImGui::CollapsingHeader(ICON_MD_VIDEOCAM "  FPSCamera", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& camera = entity_.Get<FPSCamera>();
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Position", camera.Position);
                ImGuiUtils::EditProperty("NearPlane", camera.NearPlane);
                ImGuiUtils::EditProperty("FarPlane", camera.FarPlane);
                ImGuiUtils::EditProperty("FIeldOfView", camera.FOV);
                ImGui::Columns(1);
                if (ImGui::Button("Select as main camera", {ImGui::GetContentRegionAvail().x, 20})) {
                    scene_.lock()->SetCamera(entity_);
                }
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<LightSource>() {
        if (entity_.Has<LightSource>()) {
            if (ImGui::CollapsingHeader(ICON_MD_LIGHTBULB "  LightSoruce", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto&       source        = entity_.Get<LightSource>();
                const char* light_types[] = {"Directional", "Point", "Spot"};
                ImGui::Columns(2);

                ImGui::SetCursorPosX(ImGuiUtils::Constants::kDefaultLeftPadding);
                ImGui::Text("Type");
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);
                ImGui::Combo("##Light Type", (int*)&source.Type, light_types, IM_ARRAYSIZE(light_types));
                ImGui::NextColumn();
                ImGui::Separator();

                // ImGuiUtils::EditProperty("Ambient", source.ambient, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Diffuse", source.Diffuse, ImGuiUtils::PropertyType::Color);
                ImGuiUtils::EditProperty("Specular", source.Specular, ImGuiUtils::PropertyType::Color);

                if (source.Type == LightSourceType::Point || source.Type == LightSourceType::Spot) {
                    ImGuiUtils::EditProperty("CLQ", source.CLQ);
                }
                if (source.Type == LightSourceType::Spot || source.Type == LightSourceType::Direction) {
                    ImGuiUtils::EditProperty("Direction", source.Direction);
                }
                if (source.Type == LightSourceType::Spot || source.Type == LightSourceType::Point) {
                    ImGuiUtils::EditProperty("Position", source.Position);
                }
                if (source.Type == LightSourceType::Spot) {
                    ImGuiUtils::EditProperty("Inner Cone", source.InnerConeCosinus);
                    ImGuiUtils::EditProperty("Outer Cone", source.OuterConeCosinus);
                }
                ImGui::Columns(1);
            }
        }
    }
    template <> void InspectorPanel::DrawComponentWidget<RenderMeshComponent>() {
        if (entity_.Has<RenderMeshComponent>()) {
            if (ImGui::CollapsingHeader(ICON_MD_TOKEN "  RenderMesh", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& meshes = entity_.Get<RenderMeshComponent>()->GetSubMeshes();
                int   cnt    = 0;
                for (auto& mesh : meshes) {
                    std::string name = std::format("Mesh_{}", cnt);
                    if (ImGui::TreeNode(name.c_str())) {
                        ImGui::Columns(2);
                        ImGuiUtils::EditProperty(mesh.Material);
                        ImGui::Columns(1);
                        ImGui::TreePop();
                    }
                    ++cnt;
                }
            }
        }
    }

    template <> void InspectorPanel::DrawComponentWidget<Physics::PhysicsComponent>() {
        if (entity_.Has<Physics::PhysicsComponent>()) {
            if (ImGui::CollapsingHeader("  PhysicsComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto&       phys    = entity_.Get<Physics::PhysicsComponent>();
                const float min_mas = 0;
                ImGui::Columns(2);
                ImGuiUtils::EditProperty("Speed", phys.Speed);
                ImGuiUtils::EditProperty("Rotspeed", phys.RotationSpeed);
                ImGuiUtils::EditProperty("InvMass", ImGuiDataType_Float, &phys.InvertedMass, 0.01, &min_mas);
                ImGuiUtils::EditProperty("InvInertia", ImGuiDataType_Float, &phys.InvertedInertia, 0.01, &min_mas);
                ImGuiUtils::EditProperty("Collidable", phys.Collidable);
                ImGuiUtils::EditProperty("Gravity", phys.Gravity);
                ImGui::Columns(1);
            }
        }
    }

    void InspectorPanel::OnImGui() {
        DE_PROFILE_SCOPE("InspectorPanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_INFO "  Inspector")) {
                auto scene = scene_.lock();
                if (scene && entity_.Valid()) {
                    DrawComponentWidget<TagComponent>();
                    DrawComponentWidget<ScriptComponent>();
                    DrawComponentWidget<TransformComponent>();
                    DrawComponentWidget<FPSCamera>();
                    DrawComponentWidget<LightSource>();
                    DrawComponentWidget<RenderMeshComponent>();
                    DrawComponentWidget<SkyBoxComponent>();
                    DrawComponentWidget<Physics::PhysicsComponent>();
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
            if (!entity_.Has<TransformComponent>() && ImGui::Selectable(ICON_MD_OPEN_IN_FULL "Transform")) {
                entity_.Add<TransformComponent>();
            }
            if (!entity_.Has<LightSource>() && ImGui::Selectable(ICON_MD_LIGHTBULB "LightSource")) {
                entity_.Add<LightSource>();
            }
            if (!entity_.Has<FPSCamera>() && ImGui::Selectable(ICON_MD_VIDEOCAM "Camera")) {
                entity_.Add<FPSCamera>();
            }
            ImGui::EndPopup();
        }
    }
    void InspectorPanel::SetActiveEntity(Entity entity) {
        entity_ = entity;
    }
    void InspectorPanel::SetScene(WeakRef<Scene> scene) {
        scene_ = scene;
    }

}  // namespace DummyEngine