#pragma once

#include "DummyEngine/Core/Animations/Bone.h"

namespace DE {
    class Animation {
    public:
        struct Node {
            Mat4              transformation = Mat4(1.0);
            std::string       name;
            std::vector<Node> childrens;
        };
        Animation() = default;

        BoneInfo*                                   GetBone(const std::string& name);
        float                                       GetTicksPerSecond();
        float                                       GetDuration();
        const Node&                                 GetRootNode();
        const std::unordered_map<std::string, S32>& GetBoneIDMap();

    private:
        friend class ModelLoader;

        float                                m_Duration;
        int                                  m_TicksPerSecond;
        Node                                 m_RootNode;
        std::vector<BoneInfo>                m_Bones;
        std::unordered_map<std::string, S32> m_BoneNameToID;
    };
}  // namespace DE
