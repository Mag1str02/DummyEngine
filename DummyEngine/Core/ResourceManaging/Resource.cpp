#include "Resource.hpp"

namespace DE {
    Resource::State Resource::GetState() const {
        return m_State;
    }
    UUID Resource::GetID() const {
        return m_Id;
    }
    void Resource::Prepare() {
        m_State = LOADED_TO_RAM;
    }
    void Resource::Load() {
        m_State = LOADED_TO_GPU;
    }
    void Resource::Unload() {
        m_State = LOADED_TO_RAM;
    }
    void Resource::Remove() {
        m_State = NOT_LOADED;
    }
}  // namespace DE