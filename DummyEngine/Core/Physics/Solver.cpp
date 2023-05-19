#include "Solver.hpp"

#include "PhysicsComponent.h"
void DE::Physics::Solver::LoadScene(const DE::Ref<DE::Scene>& scene) {
    _scene     = scene;
    _constants = CreateRef<SceneConstants>();
    _constants->setGravity(Vec3(0, -0.1, 0));
    _time = 0;
    LOG_INFO("Physics", "Scene loaded", scene->GetName());
}

void DE::Physics::Solver::OnUpdate(double dt) {
    double curr_dt = 0;
    while (curr_dt < dt) {
        curr_dt += NextInteraction(dt - curr_dt);
    }
}
double DE::Physics::Solver::NextInteraction(double dt) {
    for (auto entity : _scene->View<PhysicsComponent>()) {
        auto& phys      = entity.Get<PhysicsComponent>();
        auto& transform = entity.Get<TransformComponent>();
        transform.translation += phys.speed * static_cast<float>(dt);
        transform.rotation += phys.rot_speed * static_cast<float>(dt);

        if(phys.gravity) {
            phys.speed += _constants->getGravity() * static_cast<float>(dt);
        }

        if(entity.Has<RenderMeshComponent>()) {
            auto& mesh = entity.Get<RenderMeshComponent>();
            mesh.mesh_instance
        }
    }
    _time += dt;
    return dt;
}
