#pragma once

#include <GLAD/glad.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace DE {

enum class Texture2DFormat { UNKNOWN = 0, RED, RGB, RGBA };

struct Texture2DData {
    std::shared_ptr<unsigned char*> data = std::make_shared<unsigned char*>(nullptr);
    int width;
    int height;
    Texture2DFormat format;
};

struct MaterialData {
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float shininess;
    Texture2DData specular_map;
    Texture2DData diffuse_map;
    Texture2DData normal_map;
};

struct Vertex3D {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct RenderMeshData {
    std::vector<Vertex3D> vertices;
    std::vector<unsigned int> indices;
    MaterialData material;

    RenderMeshData& operator+=(const RenderMeshData& other);
};

struct RenderModelData {
    std::vector<RenderMeshData> meshes;

    void Compress();
};
}  // namespace DE