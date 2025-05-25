#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class BoidComponent {

};

class BoidsController : public Script {
    SCRIPT(GateController)
public:
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
        // return GetScene()->CreateEntity(std::format("Boid-{}", boids_.size()));
        return GetScene()->CreateEmptyEntity();
    }

private:
    std::vector<Entity> boids_;
    U32                 boids_count_ = 0;
};

SCRIPT_BASE(BoidsController, FIELD("BoidsCount", boids_count_), )
