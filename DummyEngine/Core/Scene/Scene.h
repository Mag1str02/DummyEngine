#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Scene/Components.h"

namespace DE {
    class Entity;
    class SceneRenderData;
    class SceneHierarchyNode;
    class ScripyEngine;

    class Scene {
    public:
        Scene(const std::string& name = "Scene");
        ~Scene();

        Entity CreateEmptyEntity();
        Entity CreateHiddenEntity(const std::string& name = "Entity");
        Entity CreateEntity(const std::string& name = "Entity");
        Entity CloneEntity(const std::string& entity_to_clone, const std::string& new_name);

        bool ExistsEntityWithTag(const TagComponent& name);
        bool ExistsEntityWithID(UUID id);

        Entity      GetByID(UUID uuid);
        Entity      GetByTag(const std::string& name);
        std::string GenAvilableEntityName(const std::string& prefered);

        Entity                  GetCamera();
        const std::string&      GetName() const;
        Ref<SceneHierarchyNode> GetHierarchy();

        void OnUpdate(double dt);
        void OnViewPortResize(U32 x, U32 y);
        void UpdateScripts(float dt);
        void Render();

        template <typename System> void                              RegisterSystem() { m_Storage->RegisterSystem<System>(); }
        template <typename... Components> StorageView<Components...> View() { return m_Storage->View<Components...>(); }

    private:
        friend class ScriptEngine;
        friend class SceneLoader;
        friend class SceneRenderData;

        std::string                              m_Name;
        Ref<Storage>                             m_Storage;
        Ref<SceneRenderData>                     m_RenderData;
        Ref<SceneHierarchyNode>                  m_HierarchyRoot;
        std::unordered_map<UUID, Entity>         m_EntityByID;
        std::unordered_map<TagComponent, Entity> m_EntityByTag;
    };
}  // namespace DE