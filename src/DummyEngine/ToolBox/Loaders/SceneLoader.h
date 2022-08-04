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
        static void SaveScene(Ref<Scene> scene, Path path);
        static Ref<Scene> LoadScene(Path path);

        template <typename ComponentType> static YAML::Node GetComponentNode(ComponentType component)
        {
            YAML::Node n_Component;
            n_Component["UnknownComponent"] = DemangleName<ComponentType>();
            return n_Component;
        }

    private:
        template <typename ComponentType> static void TryToSaveComponent(YAML::Node& n_Entity, Entity entity);
        static YAML::Node SaveEntity(Entity entity);
        static YAML::Node SaveModels();
        static YAML::Node SaveShaders();
        static YAML::Node SaveAssets();

        struct LoaderState
        {
            std::unordered_set<Path> m_ModelsPath;
            std::unordered_set<Ref<Shader>> m_Shaders;

            void Clear();
        };
        static LoaderState m_State;
    };

}  // namespace DE