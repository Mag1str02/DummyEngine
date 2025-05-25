#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class BoidComponent {};

class BoidsController : public Script {
    SCRIPT(BoidsController)
    LOG_AUTHOR(BoidsController)
public:
    virtual void OnAttach() override {
        LOG_INFO("Initializing boids controller");
        InitMesh();
        InitShader();
        LOG_INFO("Initialized boids controller");
    }

    virtual void OnUpdate(float) override {}
    virtual void OnRender() override {
        ChangeBoidsCount();
        Render();
    }
    virtual void OnDetach() override {
        for (auto& boid : boids_) {
            boid.Destroy();
        }
    }

private:
    void Render() {
        for (U32 i = 0; i < boids_.size(); ++i) {
            const auto& transform                = boids_[i].Get<TransformComponent>();
            instance_buffer_->At(i).Get<Mat4>(0) = transform.GetTransform();
        }
        instance_buffer_->PushData();
        boid_shader_->Bind();
        boid_shader_->Bind();
        Renderer::GetRenderAPI().DrawInstanced(boids_vao_, boids_count_);
    }
    void ChangeBoidsCount() {
        DE_PROFILE_SCOPE("BoidsController::ChangeBoidsCount");
        if (kBoidsLimit < boids_count_) {
            boids_count_ = kBoidsLimit;
        }
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
    void InitMesh() {
        auto mesh_data = ResourceManager::GetRenderMeshData(UUID("02afee3bc1cb75b7d8b882a8667faa84"));
        if (!mesh_data) {
            LOG_ERROR("No mesh data for boid");
            return;
        }
        if (mesh_data.value()->Meshes.size() != 1) {
            LOG_ERROR("Invalid submeshes amount");
            return;
        }
        const auto& mesh = mesh_data.value()->Meshes.front();

        {
            BufferLayout main_layout({
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float2,
                BufferElementType::Int4,
                BufferElementType::Int4,
                BufferElementType::Float4,
                BufferElementType::Float4,
            });
            BufferLayout instance_layout({BufferElementType::Mat4}, 1);

            Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(main_layout, mesh.Vertices.size(), &mesh.Vertices[0]);
            Ref<IndexBuffer>  index_buffer  = IndexBuffer::Create(&mesh.Indices[0], mesh.Indices.size());

            instance_buffer_ = VertexBuffer::Create(instance_layout, kBoidsLimit, BufferUsage::Dynamic);

            boids_vao_ = VertexArray::Create();
            boids_vao_->AddVertexBuffer(vertex_buffer);
            boids_vao_->AddVertexBuffer(instance_buffer_);
            boids_vao_->SetIndexBuffer(index_buffer);
        }
    }
    void InitShader() {
        auto id = UUID("24116af57b6022cd2464fbf641ae945c");
        if (ResourceManager::HasShader(id)) {
            ResourceManager::DeleteShader(id);
        }
        if (!ResourceManager::LoadShader(id)) {
            LOG_ERROR("Failed to load shader");
            return;
        }

        GetScene()->GetRenderer()->ForgetShader(id);
        GetScene()->GetRenderer()->RequestShader(id);
        auto shader = ResourceManager::GetShader(id);
        if (!shader) {
            LOG_ERROR("No shader for boid");
            return;
        }
        boid_shader_ = shader.value();
    }

    Entity CreateBoid() {
        DE_PROFILE_SCOPE("BoidsController::CreateBoid");
        auto boid = GetScene()->CreateEmptyEntity();

        auto& transform       = boid.Add<TransformComponent>();
        transform.Translation = bounding_box_center_;
        transform.Translation.x += Random::Float(-(float)bounding_box_size_, +bounding_box_size_);
        transform.Translation.y += Random::Float(-(float)bounding_box_size_, +bounding_box_size_);
        transform.Translation.z += Random::Float(-(float)bounding_box_size_, +bounding_box_size_);
        transform.Scale = Vec3(Random::Float(0.5, 1));

        LOG_INFO("Created boid at pos {} with scale {}", transform.Translation, transform.Scale);

        return boid;
    }

private:
    static constexpr U32 kBoidsLimit = 10'000;

    U32  boids_count_         = 0;
    Vec3 bounding_box_center_ = Vec3(0.0f);
    U32  bounding_box_size_   = 10;

    std::vector<Entity> boids_;

    Ref<Shader>       boid_shader_;
    Ref<VertexArray>  boids_vao_;
    Ref<VertexBuffer> instance_buffer_;
};

SCRIPT_BASE(BoidsController,
            FIELD("BoidsCount", boids_count_),                 //
            FIELD("BoundingBoxSize", bounding_box_size_),      //
            FIELD("BoundingBoxCenter", bounding_box_center_),  //
)
