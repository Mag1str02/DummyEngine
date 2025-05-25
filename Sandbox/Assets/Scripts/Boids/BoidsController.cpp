#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class BoidComponent {};

class BoidsController : public Script {
    SCRIPT(BoidsController)
    LOG_AUTHOR(BoidsController)
public:
    virtual void OnAttach() override {
        LOG_INFO("Initializing boids controller");
        mesh_.ID   = UUID("02afee3bc1cb75b7d8b882a8667faa84");
        shader_.ID = UUID("24116af57b6022cd2464fbf641ae946a");

        auto shader = ResourceManager::GetShader(shader_.ID);
        if (!shader) {
            LOG_ERROR("No shader for boid");
            return;
        }
        auto mesh_data = ResourceManager::GetRenderMeshData(mesh_.ID);
        if (!mesh_data) {
            LOG_ERROR("No mesh data for boid");
            return;
        }

        shader_.Shader = shader.value();
        mesh_.Mesh     = CreateRef<RenderMesh>(mesh_data.value());
        for (auto& mesh : mesh_.Mesh->GetSubMeshes()) {
            mesh.Material.ORM = Vec3(0.0, 0.4, 0.0);
            mesh.Material.Ambient = Vec3(1.0);
        }
    }

    virtual void OnUpdate(float) override {}
    virtual void OnRender() override { ChangeBoidsCount(); }

private:
    void ChangeBoidsCount() {
        DE_PROFILE_SCOPE("BoidsController::ChangeBoidsCount");
        bool changed = boids_count_ != boids_.size();
        if (changed) {
            LOG_INFO("Updating boids count from {} to {}", boids_.size(), boids_count_);
        }
        while (boids_count_ < boids_.size()) {
            boids_.back().Destroy();
            boids_.pop_back();
        }
        while (boids_count_ > boids_.size()) {
            boids_.emplace_back(CreateBoid());
        }
        if (changed) {
            LOG_INFO("Updated boids count");
        }
    }
    Entity CreateBoid() {
        DE_PROFILE_SCOPE("BoidsController::CreateBoid");
        auto boid = GetScene()->CreateEmptyEntity();

        auto& transform       = boid.Add<TransformComponent>();
        transform.Translation = bounding_box_center_;
        transform.Translation.x += Random::Float(-(float)bounding_box_size_, +bounding_box_size_);
        transform.Translation.y += Random::Float(-(float)bounding_box_size_, +bounding_box_size_);
        transform.Translation.z += Random::Float(-(float)bounding_box_size_, +bounding_box_size_);

        LOG_INFO("Created boid at pos {}", transform.Translation);

        if (mesh_.Mesh) {
            boid.Add<RenderMeshComponent>(mesh_);
        }
        if (shader_.Shader) {
            boid.Add<ShaderComponent>(shader_);
        }

        return boid;
    }

private:
    std::vector<Entity> boids_;
    U32                 boids_count_ = 0;
    Vec3                bounding_box_center_ = Vec3(0.0f);
    U32                 bounding_box_size_ = 10;

    RenderMeshComponent mesh_;

    ShaderComponent shader_;
};

SCRIPT_BASE(BoidsController,
            FIELD("BoidsCount", boids_count_),                 //
            FIELD("BoundingBoxSize", bounding_box_size_),      //
            FIELD("BoundingBoxCenter", bounding_box_center_),  //
)
