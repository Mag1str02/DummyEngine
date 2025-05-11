#pragma once

#include "DummyEngine/Core/Animations/Bone.h"

#include <unordered_map>

namespace DummyEngine {

    class Animation {
    public:
        struct Node {
            Mat4              Transformation = Mat4(1.0);
            std::string       Name;
            std::vector<Node> Childrens;
        };
        Animation() = default;

        BoneInfo*                                   GetBone(const std::string& name);
        float                                       GetTicksPerSecond();
        float                                       GetDuration();
        const Node&                                 GetRootNode();
        const std::unordered_map<std::string, S32>& GetBoneIDMap();

    private:
        friend class ModelLoader;

        float                                duration_;
        int                                  ticks_per_second_;
        Node                                 root_node_;
        std::vector<BoneInfo>                bones_;
        std::unordered_map<std::string, S32> bone_name_to_id_;
    };

}  // namespace DummyEngine
