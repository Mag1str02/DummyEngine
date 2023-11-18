//
// Created by balanda on 10/17/2023.
//

#include "ShaderPart.hpp"

#include "DummyEngine/Core/Application/Config.h"

namespace DE {
    void ShaderPartResource::Prepare() {
        std::string line;
        m_Source.resize(0);
        m_Source.shrink_to_fit();

        std::ifstream fin(m_Path);
        if (!fin.is_open()) {
            LOG_ERROR("ShaderPartResource::Prepare", "Can't open shader source file(", RelativeToExecutable(m_Path), ")");
        }
        try {
            while (getline(fin, line)) { // TODO: include in shader source
                m_Source.append(line + '\n');
            }
        } catch (...) {
            LOG_ERROR("ShaderPartResource", "Failed to read shader source file (", RelativeToExecutable(m_Path), ")");;
        }
        Resource::Prepare();
    }
    void ShaderPartResource::Load() {
        m_ShaderPart = ShaderPart::Create(shared_from_this());
        Resource::Load();
    }
    void ShaderPartResource::Unload() {
        m_ShaderPart = nullptr;
        Resource::Unload();
    }
    void ShaderPartResource::Remove() {
        m_Source.resize(0);
        m_Source.shrink_to_fit();
        Resource::Remove();
    }
    ShaderPartResource::~ShaderPartResource() {
        if (GetState() == LOADED_TO_GPU) {
            Unload();
        }
        if (GetState() == LOADED_TO_RAM) {
            Remove();
        }
        DE_ASSERT(GetState() == NOT_LOADED, "ShaderPart didn't unload");
    }
    ShaderPartResource::ShaderPartResource(Path path, ShaderPartType type) : m_Path(path), m_Type(type) {}
}  // namespace DE