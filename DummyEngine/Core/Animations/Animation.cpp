#include "DummyEngine/Core/Animations/Animation.h"

namespace DummyEngine {

    BoneInfo* Animation::GetBone(const std::string& name) {
        return (bone_name_to_id_.contains(name) ? &bones_[bone_name_to_id_[name]] : nullptr);
    }
    float Animation::GetTicksPerSecond() {
        return ticks_per_second_;
    }
    float Animation::GetDuration() {
        return duration_;
    }
    const Animation::Node& Animation::GetRootNode() {
        return root_node_;
    }
    const std::unordered_map<std::string, S32>& Animation::GetBoneIDMap() {
        return bone_name_to_id_;
    }

}  // namespace DummyEngine
