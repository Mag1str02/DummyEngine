#pragma once

#include "Addition/Base.h"
#include "Core/Scene/Components.h"
#include "Core/Scene/Scene.h"

namespace DE
{

    class SceneLoader
    {
    public:
        static void SaveScene(Ref<Scene> scene, Path path);
        static Ref<Scene> LoadScene(Path path);
    };

    template <typename ComponentType> YAML::Node GetComponentNode(ComponentType component)
    {
        YAML::Node n_Component;
        n_Component["UnknownComponent"] = DemangleName<ComponentType>();
        return n_Component;
    }

    template <> YAML::Node GetComponentNode(Tag component)
    {
        YAML::Node n_Component;
        n_Component["Tag"] = component.tag;
        return n_Component;
    }
    template <> YAML::Node GetComponentNode(Id component)
    {
        YAML::Node n_Component;
        n_Component["UUID"] = (uint64_t)component;
        return n_Component;
    }
    template <> YAML::Node GetComponentNode(Transformation component)
    {
        YAML::Node n_Component;
        n_Component["Transformation"]["Translation"] = NodeVec3(component.translation);
        n_Component["Transformation"]["TranslationOffset"] = NodeVec3(component.translation_offset);
        n_Component["Transformation"]["Rotation"] = NodeVec3(component.rotation);
        n_Component["Transformation"]["RotationOffset"] = NodeVec3(component.rotation_offet);
        n_Component["Transformation"]["Scale"] = NodeVec3(component.scale);
        n_Component["Transformation"]["ScaleOffset"] = NodeVec3(component.scale_offset);
        return n_Component;
    }

}  // namespace DE