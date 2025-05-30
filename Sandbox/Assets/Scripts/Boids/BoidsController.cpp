#include "DummyEngine/DummyEngine.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>

using namespace DummyEngine;

class BoidsController;

class BoidComponent {
public:
    explicit BoidComponent(Vec3 pos, Vec3 direction, float scale, BoidsController* controller, U32 boid_id);
    virtual ~BoidComponent() = default;

    void ObserveOthers();
    void Move(float dt);
    void UpdateTransform();

private:
    float scale_;
    Vec3  pos_;
    Vec3  direction_;
    Vec3  new_direction_;

    BoidsController* controller_;
    U32              boid_id_;
};

enum class UpdateType {
    Observe,
    Move,
    Transform,
};

class BoidsController : public Script {
    SCRIPT(BoidsController)
    LOG_AUTHOR(BoidsController)
public:
    virtual void OnAttach() override {
        LOG_INFO("Initializing boids controller");
        InitMesh();
        InitShader();
        LOG_INFO("Initialized boids controller");

        auto storage = GetStorage();
    }

    virtual void OnUpdate(float dt) override {
        UpdateBoids(UpdateType::Observe, dt);
        UpdateBoids(UpdateType::Move, dt);
        UpdateBoids(UpdateType::Transform, dt);
    }
    virtual void OnRender() override {
        ChangeBoidsCount();
        Render();
    }
    virtual void OnDetach() override { LOG_INFO("Destroyed boids component"); }

    Mat4& GetTransfrom(U32 boid_id) { return instance_buffer_->At(boid_id).Get<Mat4>(0); }

private:
    void UpdateBoids(UpdateType type, float dt) {
        DE_PROFILE_SCOPE("BoidsController::Update");
        U32        size       = boids_.size();
        U32        group_size = std::max(size / std::thread::hardware_concurrency() / 4, 200u);
        FWaitGroup wg;
        for (U32 i = 0; i < size; i += group_size) {
            wg.Add(1);
            Runtime::Submit(Concurrency::GetEngineBackgroundScheduler(), [this, type, dt, i, group_size, &wg] {
                DE_PROFILE_SCOPE("BoidsController::GroupUpdate");
                U32 end = std::min<U32>(boids_.size(), i + group_size);
                for (U32 k = i; k < end; ++k) {
                    auto& comp = boids_[k];
                    switch (type) {
                        case UpdateType::Transform: comp.UpdateTransform(); break;
                        case UpdateType::Move: comp.Move(dt); break;
                        case UpdateType::Observe: comp.ObserveOthers(); break;
                    }
                }
                wg.Done();
            });
        }
        wg.Wait();
    }

    void Render() {
        DE_PROFILE_SCOPE("BoidsController::Render");
        boid_material_.Albedo = albedo_;

        {
            DE_PROFILE_SCOPE("BoidsController::Render (PushData)");

            instance_buffer_->PushData();
        }
        {
            DE_PROFILE_SCOPE("BoidsController::Render (InstancedRender)");
            boid_shader_->Bind();
            boid_material_.Apply(boid_shader_);

            Renderer::GetRenderAPI().DrawInstanced(boids_vao_, boids_count_);
        }
    }
    void ChangeBoidsCount() {
        DE_PROFILE_SCOPE("BoidsController::ChangeBoidsCount");
        if (kBoidsLimit < boids_count_) {
            boids_count_ = kBoidsLimit;
        }
        bool changed = boids_count_ != boids_.size();
        while (boids_count_ < boids_.size()) {
            boids_.pop_back();
        }
        while (boids_count_ > boids_.size()) {
            boids_.emplace_back(CreateBoid());
        }
        if (changed) {
            UpdateBoids(UpdateType::Transform, 0);
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
        boid_material_.Ambient = Vec3(0.2);
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
    BoidComponent CreateBoid() {
        DE_PROFILE_SCOPE("BoidsController::CreateBoid");
        auto pos       = glm::linearRand(bounding_box_center_ - Vec3(bounding_box_size_), bounding_box_center_ + Vec3(bounding_box_size_));
        auto direction = glm::sphericalRand(1.0);
        auto scale     = Random::Float(0.5, 1);
        return BoidComponent{pos, direction, scale, this, (U32)boids_.size()};
    }

private:
    friend class BoidComponent;
    static constexpr U32 kBoidsLimit = 10'000;

    U32   boids_count_         = 0;
    Vec3  bounding_box_center_ = Vec3(0.0f);
    Vec3  albedo_              = Vec3(1.0f);
    float bounding_box_size_   = 50;
    float speed_               = 1;

    float random_weight_    = 1.0f;
    float initial_weight_   = 1.0f;
    float alignment_weight_ = 1.0f;
    float avoid_weight_     = 1.0f;
    float center_weight_    = 1.0f;
    float border_weight_    = 1.0f;
    float alignment_radius_ = 50.0f;
    float avoid_radius_     = 50.0f;

    std::vector<BoidComponent> boids_;

    Ref<Shader>       boid_shader_;
    Ref<VertexArray>  boids_vao_;
    Ref<VertexBuffer> instance_buffer_;
    Material          boid_material_;
};

SCRIPT_BASE(BoidsController,
            FIELD("Boids Count", boids_count_),                 //
            FIELD("BoundingBox Size", bounding_box_size_),      //
            FIELD("BoundingBox Center", bounding_box_center_),  //
            FIELD("Albedo", albedo_),                           //

            FIELD("Alignment Radius", alignment_radius_),  //
            FIELD("Avoid Radius", avoid_radius_),          //
            FIELD("Speed", speed_),                        //

            FIELD("Weight Random", random_weight_),        //
            FIELD("Weight Initial", initial_weight_),      //
            FIELD("Weight Alignment", alignment_weight_),  //
            FIELD("Weight Center", center_weight_),        //
            FIELD("Weight Border", border_weight_),        //
            FIELD("Weight Avoid", avoid_weight_),          //
)

BoidComponent::BoidComponent(Vec3 pos, Vec3 direction, float scale, BoidsController* controller, U32 boid_id) :
    scale_(scale), pos_(pos), direction_(direction), controller_(controller), boid_id_(boid_id) {}

void BoidComponent::UpdateTransform() {
    controller_->GetTransfrom(boid_id_) =
        glm::translate(pos_) * glm::toMat4(glm::quatLookAt(direction_, Vec3(0.0, 1.0, 0.0))) * glm::scale(Vec3(scale_));
}

void BoidComponent::Move(float dt) {
    direction_ = new_direction_;
    pos_ += direction_ * dt * controller_->speed_;

    // auto box_size = Vec3(controller_->bounding_box_size_);
    // auto box_min  = controller_->bounding_box_center_ - box_size;

    // pos_ = glm::mod(pos_ - box_min, box_size * 2.0f) + box_min;
}
void BoidComponent::ObserveOthers() {
    DE_PROFILE_SCOPE("BoidComponent::ObserveOthers");
    // float total_weight = 0.001;
    Vec3 total_vec = Vec3(0.0);

    // Current direction
    {
        // total_weight += controller_->initial_weight_;
        total_vec += direction_ * controller_->initial_weight_;
    }

    // Random
    {
        // total_weight += controller_->random_weight_;
        total_vec += glm::sphericalRand<float>(1.0) * controller_->random_weight_;
    }

    // Border
    {
        // total_weight += controller_->random_weight_;

        auto box_size = Vec3(controller_->bounding_box_size_);
        auto box_min  = controller_->bounding_box_center_ - box_size;

        glm::vec3 clamped    = glm::clamp(pos_, box_min, box_min + box_size * 2.0f);
        auto      center_vec = (clamped - pos_);
        total_vec += center_vec * controller_->border_weight_;
    }

    // Align and avoid
    {
        DE_PROFILE_SCOPE("BoidComponent::ObserveOthers (A&A)");
        Vec3 alignment(0.0f);
        Vec3 avoid(0.0f);
        Vec3 center(0.0f);
        U32  count_alignment = 0;
        U32  count_avoid     = 0;

        for (auto& boid : controller_->boids_) {
            Vec3  pos_delta = pos_ - boid.pos_;
            float distance  = glm::length(pos_delta);
            center += boid.pos_;

            if (distance < controller_->alignment_radius_) {
                float strength = (controller_->alignment_radius_ - distance) / controller_->alignment_radius_;
                alignment += boid.direction_ * strength;
                ++count_alignment;
            }
            if (distance < controller_->avoid_radius_) {
                float strength = (controller_->avoid_radius_ - distance) / controller_->avoid_radius_;
                avoid += pos_delta * strength;
                ++count_avoid;
            }
        }
        alignment /= count_alignment;
        avoid /= count_avoid;
        center /= controller_->boids_count_;

        total_vec += alignment * controller_->alignment_weight_;
        total_vec += avoid * controller_->avoid_weight_;
        total_vec += (center - pos_) * controller_->center_weight_;
    }

    new_direction_ = glm::normalize(total_vec);
}
