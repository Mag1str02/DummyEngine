#pragma once

namespace DummyEngine {
#ifdef ECS_IMPLEMENTATION

    Storage::Storage() : component_manager_(this), system_manager_(this) {}
    void Storage::Destruct() {
        for (U32 id = entity_manager_.BeginEntity(); id != entity_manager_.EndEntity(); id = entity_manager_.NextEntity(id)) {
            component_manager_.Destroy(id);
            entity_manager_.Destroy(id);
        }
    }
    Entity Storage::CreateEntity() {
        auto [id, gen] = entity_manager_.CreateEntity();
        Entity ent;
        ent.id_      = id;
        ent.gen_     = gen;
        ent.storage_ = weak_from_this();
        return ent;
    }
    void Storage::UpdateSystems(float dt) {
        system_manager_.Update(dt);
    }
    Entity Storage::GetEntity(U32 id) {
        Entity res;
        res.id_      = id;
        res.gen_     = entity_manager_.Generation(id);
        res.storage_ = weak_from_this();
        return res;
    }
    bool Storage::Valid(U32 id, U32 gen) const {
        return entity_manager_.Valid(id, gen);
    }
    void Storage::Destroy(U32 id, U32 gen) {
        if (entity_manager_.Valid(id, gen)) {
            component_manager_.Destroy(id);
            entity_manager_.Destroy(id);
        }
    }
#endif
    template <typename... Components> StorageView<Components...> Storage::View() {
        return StorageView<Components...>(this);
    }

    template <typename ComponentType> void Storage::SetAddHandler(std::function<void(Entity)> func) {
        component_manager_.SetAddHandler<ComponentType>(func);
    }
    template <typename ComponentType> void Storage::SetRemoveHandler(std::function<void(Entity)> func) {
        component_manager_.SetRemoveHandler<ComponentType>(func);
    }

    template <typename SystemType> void Storage::AttachSystem(std::shared_ptr<System> system) {
        system_manager_.AttachSystem<SystemType>(system);
    }

    template <typename ComponentType> ComponentType* Storage::AddComponent(U32 id, U32 gen, const ComponentType& component) {
        return (entity_manager_.Valid(id, gen) ? component_manager_.AddComponent<ComponentType>(id, component) : nullptr);
    }
    template <typename ComponentType> ComponentType* Storage::GetComponent(U32 id, U32 gen) {
        return (entity_manager_.Valid(id, gen) ? component_manager_.GetComponent<ComponentType>(id) : nullptr);
    }
    template <typename ComponentType> bool Storage::HasComponent(U32 id, U32 gen) const {
        return (entity_manager_.Valid(id, gen) ? component_manager_.HasComponent<ComponentType>(id) : false);
    }
    template <typename ComponentType> void Storage::RemoveComponent(U32 id, U32 gen) {
        if (entity_manager_.Valid(id, gen)) {
            component_manager_.RemoveComponent<ComponentType>(id);
        }
    }

}  // namespace DummyEngine