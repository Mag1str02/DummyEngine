#include "DummyEngine/Core/Animations/Animation.h"

namespace DE {
    BoneInfo* Animation::GetBone(const std::string& name) {
        return (m_BoneNameToID.contains(name) ? &m_Bones[m_BoneNameToID[name]] : nullptr);
    }
    float Animation::GetTicksPerSecond() {
        return m_TicksPerSecond;
    }
    float Animation::GetDuration() {
        return m_Duration;
    }
    const Animation::Node& Animation::GetRootNode() {
        return m_RootNode;
    }
    const std::unordered_map<std::string, S32>& Animation::GetBoneIDMap() {
        return m_BoneNameToID;
    }
}  // namespace DE
