#include "DummyEngine/Utils/Types/Types.h"

#include "DummyEngine/Utils/Debug/Logger.h"

namespace DE {
    void Vertex3D::AddBone(S32 id, float weight) {
        for (size_t i = 0; i < MAX_BONE_INFLUENCE; ++i) {
            if (bones[i] == -1) {
                bones[i]   = id;
                weights[i] = weight;
                return;
            }
        }
    }
}  // namespace DE
