#include "DummyEngine/Utils/Conversions.h"

namespace DE
{
    std::ostream& operator<<(std::ostream& os, const Vec2& vec)
    {
        os << "(" << vec.x << " " << vec.y << ")";
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const Vec3& vec)
    {
        os << "(" << vec.x << " " << vec.y << " " << vec.z << ")";
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const Vec4& vec)
    {
        os << "(" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << ")";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mat4& mat)
    {
        os << "((" << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << ") (" << mat[0][1] << " " << mat[1][1] << " "
           << mat[2][1] << " " << mat[3][1] << ") (" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << ") (" << mat[0][3]
           << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << "))";
        return os;
    }
}  // namespace DE