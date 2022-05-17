#include "texture_2D.h"
namespace DE {
std::string Texture2DTypeToString(Texture2DType type)
{
    switch (type)
    {
    default:
        return "uninitialized";
    case Texture2DType::diffuse:
        return "diffuse";
    case Texture2DType::specular:
        return "specular";
    }
}
Texture2D::Texture2D()
{
    _texture_id = 0;
    _type = Texture2DType::uninitialized;
}
Texture2D::Texture2D(unsigned int id, Texture2DType type)
{
    _texture_id = id;
    _type = type;
}

void Texture2D::BindToUnit(unsigned int unit_id) const
{
    glActiveTexture(GL_TEXTURE0 + unit_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}
Texture2DType Texture2D::GetType() const
{
    return _type;
}
}