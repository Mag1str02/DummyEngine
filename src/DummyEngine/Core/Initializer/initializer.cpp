#include "DummyEngine/Core/Initializer/initializer.h"
#include "DummyEngine/Core/ECS/component_manager.hpp"
#include "DummyEngine/Core/ECS/entity_manager.hpp"
#include "DummyEngine/Core/ECS/system_manager.hpp"

namespace DE {

Initializer::Initializer() {
    _max_entity_amount = 1000;
}
Initializer& Initializer::Get() {
    static Initializer initializer;
    return initializer;
}

void Initializer::SetHint(int64_t hint_id, int64_t hint_value) {
    switch (hint_id) {
        default:
            break;
    }
}

void Initializer::Initialize() {
    EntityManager::Get().Initialize();
    ComponentManager::Get();
}
void Initializer::Terminate() {
    EntityManager::Get().Terminate();
    ComponentManager::Get().Terminate();
    SystemManager::Terminate();
}

void deInitialize() {
    Initializer::Get().Initialize();
}
void deTerminate() {
    Initializer::Get().Terminate();
}

void deHint(int64_t hint_id, int64_t hint_value) {
    Initializer::Get().SetHint(hint_id, hint_value);
}
}  // namespace DE
