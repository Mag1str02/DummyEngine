#include "DummyEngine/Core/Initializer/Initializer.h"
#include "DummyEngine/Core/ECS/ComponentManager.hpp"
#include "DummyEngine/Core/ECS/EntityManager.hpp"
#include "DummyEngine/Core/ECS/SystemManager.hpp"

namespace DE {

Initializer::Initializer() {
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
