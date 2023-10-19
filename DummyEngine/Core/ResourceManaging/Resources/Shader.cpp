//
// Created by balanda on 10/18/2023.
//

#include "Shader.hpp"

namespace DE {
    void ShaderResource::Prepare() {
        Resource::Prepare();
    }
    void ShaderResource::Load() {
        Resource::Load();
    }
    void ShaderResource::Unload() {
        Resource::Unload();
    }
    void ShaderResource::Remove() {
        Resource::Remove();
    }
    void ShaderResource::AddPart(Ref<ShaderPartResource>& part) {
        DE_ASSERT(GetState() == NOT_LOADED, "Can't add ShaderPart to prepared Shader");
        m_Parts.push_back(part);
    }
}  // namespace DE