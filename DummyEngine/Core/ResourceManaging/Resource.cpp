#include "Resource.hpp"

#include "DummyEngine/Utils/Debug/Assert.h"

namespace DE {
    Resource::State Resource::GetState() const {
        return m_State;
    }
    UUID Resource::GetID() const {
        return m_Id;
    }
    void Resource::Prepare() {
        DE_ASSERT(m_State == NOT_LOADED, "Trying to prepare already loaded resource");
        m_State = LOADED_TO_RAM;
    }
    void Resource::Load() {
        DE_ASSERT(m_State == LOADED_TO_RAM, "Trying to load to GPU resource in invalid state");
        m_State = LOADED_TO_GPU;
    }
    void Resource::Unload() {
        DE_ASSERT(m_State == LOADED_TO_GPU, "Trying to unload from GPU resource that hasn't been loaded");
        m_State = LOADED_TO_RAM;
    }
    void Resource::Remove() {
        DE_ASSERT(m_State == LOADED_TO_RAM, "Trying to remove resource in invalid state");
        m_State = NOT_LOADED;
    }
}  // namespace DE