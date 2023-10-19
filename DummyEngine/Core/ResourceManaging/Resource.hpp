#pragma once
#include "DummyEngine/Utils/Types/Types.h"
#include "DummyEngine/Utils/Types/UUID.h"

namespace DE {
    class Resource {
    public:
        enum State { NOT_LOADED = 0, LOADED_TO_RAM = 1, LOADED_TO_GPU = 2 };

        State GetState() const;
        UUID GetID() const;

        virtual void Prepare(); // Loads all data to RAM
        virtual void Load(); // Loads needed data to GPU
        virtual void Unload();
        virtual void Remove();

    private:
        State m_State;
        UUID m_Id;
    };
}