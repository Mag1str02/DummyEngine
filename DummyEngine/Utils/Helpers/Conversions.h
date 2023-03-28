#pragma once

#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {
    std::ostream& operator<<(std::ostream& os, const Vec2& vec);
    std::ostream& operator<<(std::ostream& os, const Vec3& vec);
    std::ostream& operator<<(std::ostream& os, const Vec4& vec);
    std::ostream& operator<<(std::ostream& os, const Mat4& vec);
}  // namespace DE