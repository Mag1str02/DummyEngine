#include "Shader.hpp"

namespace DE {
    void ShaderResource::Prepare() {
        for (Ref<ShaderPartResource>& part : m_Parts) {
            DE_ASSERT(part->GetState() == NOT_LOADED, "All ShaderParts should be loaded");
        }
        Resource::Prepare();
    }
    void ShaderResource::Load() {
        std::vector<Ref<ShaderPart>> parts;
        for (Ref<ShaderPartResource>& part : m_Parts) {
            DE_ASSERT(part->GetState() == NOT_LOADED, "All ShaderParts should be loaded");
            if (part->GetState() == LOADED_TO_RAM) {
                part->Load();
            }
            parts.push_back(part->m_ShaderPart);
        }
        m_Shader = Shader::Create(parts);
        Resource::Load();
    }
    void ShaderResource::Unload() {
        m_Shader->Unload();
        Resource::Unload();
    }
    void ShaderResource::Remove() {
        m_Parts.resize(0);
        m_Parts.shrink_to_fit();
        Resource::Remove();
    }

    void ShaderResource::AddPart(Ref<ShaderPartResource>& part) {
        DE_ASSERT(GetState() == NOT_LOADED, "Can't add ShaderPart to prepared Shader");
        m_Parts.push_back(part);
    }
    ShaderResource::~ShaderResource() {
        if (GetState() == LOADED_TO_GPU) {
            Unload();
        }
        if (GetState() == LOADED_TO_RAM) {
            Remove();
        }
        DE_ASSERT(GetState() == NOT_LOADED, "Shader didn't unload");
    }
}  // namespace DE