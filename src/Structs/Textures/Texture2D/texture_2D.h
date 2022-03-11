#pragma once
#include <GLAD/glad.h>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

enum Texture2DType
{
    uninitialized,
    diffuse,
    specular,
    emission,
    normal,
    metallic,
    roughness,
    albedo,
    ao
};

std::string Texture2DTypeToString(Texture2DType type);

struct Texture2D
{
private:
    unsigned int _texture_id;
    Texture2DType _type = Texture2DType::uninitialized;

public:
    Texture2D(unsigned int id, Texture2DType type);

    void BindToUnit(unsigned int unit_id) const;
    Texture2DType GetType() const;
};