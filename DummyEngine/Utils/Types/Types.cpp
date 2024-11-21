#include "Types.h"

namespace DummyEngine {

    void Vertex3D::AddBone(S32 id, float weight) {
        for (size_t i = 0; i < MAX_BONE_INFLUENCE; ++i) {
            if (Bones[i] == -1) {
                Bones[i]   = id;
                Weights[i] = weight;
                return;
            }
        }
    }

}  // namespace DummyEngine
