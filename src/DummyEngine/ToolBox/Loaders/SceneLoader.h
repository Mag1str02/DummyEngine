#pragma once

#include "Addition/Base.h"
#include "Core/Scene/Components.h"
#include "Core/Scene/Scene.h"

namespace DE
{
    YAML::Node NodeVec2(Vec2 vec);
    YAML::Node NodeVec3(Vec3 vec);
    YAML::Node NodeVec4(Vec4 vec);

    class SceneLoader
    {
    public:
        static void Save(Ref<Scene> scene, Path path);
        static void Load(Ref<Scene> scene, Path path);

        template <typename ComponentType> static void SaveComponent(YAML::Node n_Entity, Entity entity);
        template <typename ComponentType> static void LoadComponent(YAML::Node n_Component, Entity& entity);

    private:
        static void SaveEntity(YAML::Node n_Entity, Entity entity);
        static YAML::Node SaveEntities(Ref<Scene> scene);
        static YAML::Node SaveModels();
        static YAML::Node SaveShaders();
        static YAML::Node SaveAssets();

        static void LoadShaders(YAML::Node n_Shaders);
        static void LoadModels(YAML::Node n_Models);
        static void LoadAssets(YAML::Node n_Assets);
        static void LoadEntity(YAML::Node n_Entity);
        static void LoadEntities(YAML::Node n_Entities);

        struct LoaderState
        {
            //*~~~Saving~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            std::vector<Entity> m_SShaders;
            std::vector<Entity> m_SModels;

            //*~~~Loading~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            std::unordered_map<std::string, Ref<Shader>> m_LShaders;

            Ref<Scene> m_Scene;
            void Clear();
        };
        static LoaderState m_State;
    };

}  // namespace DE