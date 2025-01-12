#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <iostream>

namespace DummyEngine {

    std::ostream& operator<<(std::ostream& os, const Vec2& vec);
    std::ostream& operator<<(std::ostream& os, const Vec3& vec);
    std::ostream& operator<<(std::ostream& os, const Vec4& vec);
    std::ostream& operator<<(std::ostream& os, const Mat4& vec);

}  // namespace DummyEngine