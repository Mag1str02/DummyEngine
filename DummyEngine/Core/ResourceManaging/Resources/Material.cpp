#include "Material.hpp"

#include "DummyEngine/Utils/Debug/Assert.h"

namespace DE {
    std::string MaterialTypeToStr(MaterialType type) {
        switch (type) {
            case MaterialType::PBR: return "PBR";
            case MaterialType::Phong: return "Phong";
            case MaterialType::None: return "None";
            default: DE_ASSERT(false, "Unsupported material type");
        }
        return "None";
    }
    MaterialType MaterialTypeFromStr(const std::string& str) {
        if (str == "PBR") return MaterialType::PBR;
        if (str == "Phong") return MaterialType::Phong;
        return MaterialType::None;
    }
    void Material::Prepare() {
        DE_ASSERT(false, "Can't prepare material outside ModelLoader");
        Resource::Prepare();
    }
    void Material::Load() {
        DE_ASSERT(false, "Can't load material to GPU");
        Resource::Load();
    }
    void Material::Unload() {
        DE_ASSERT(false, "Can't load material to GPU");
        Resource::Unload();
    }
    void Material::Remove() {
        DE_ASSERT(false, "Can't prepare material outside ModelLoader");
        Resource::Remove();
    }
}