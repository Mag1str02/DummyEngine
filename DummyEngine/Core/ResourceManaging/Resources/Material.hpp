#pragma once

#include "DummyEngine/Core/ResourceManaging/Resource.hpp"

namespace DE {

    enum class MaterialType {
        None = 0,
        Phong,
        PBR,
    };
    std::string  MaterialTypeToStr(MaterialType type);
    MaterialType MaterialTypeFromStr(const std::string& str);

    class Material : public Resource {
    public:
        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

        MaterialType type = MaterialType::PBR;
        float        shininess;

        Vec3 albedo   = Vec3(1.0f);
        Vec3 diffuse  = Vec3(1.0f);
        Vec3 specular = Vec3(1.0f);
        Vec3 orm      = Vec3(1.0f);
        Vec3 ambient  = Vec3(1.0f);
        Vec3 emission = Vec3(1.0f);

        Ref<Resource> albedo_map;
        Ref<Resource> diffuse_map;
        Ref<Resource> specular_map;
        Ref<Resource> normal_map;
        Ref<Resource> orm_map;
        Ref<Resource> emission_map;
    private:
    };

};