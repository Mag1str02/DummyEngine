#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"

namespace DE
{

    class ScriptClass
    {
    public:
        using FConstructor = void (*)(void*);
        using FDestructor  = void (*)(void*);
        using FOnCreate    = void (*)(void*);
        using FOnUpdate    = void (*)(void*, float);
        using FOnDestroy   = void (*)(void*);
        using FAlignOf     = uint32_t (*)();
        using FSizeOf      = uint32_t (*)();

        ScriptClass(const std::string& name);

        void Load(Ref<SharedObject> library);
        bool Valid();

        inline void     Construct(void* ptr) { constructor(ptr); }
        inline void     Destruct(void* ptr) { destructor(ptr); }
        inline void     OnCreate(void* ptr) { on_create(ptr); }
        inline void     OnUpdate(void* ptr, float dt) { on_update(ptr, dt); }
        inline void     OnDestroy(void* ptr) { on_destroy(ptr); }
        inline uint32_t AlignOf() { return align_of(); }
        inline uint32_t SizeOf() { return size_of(); }

    private:
        std::string       m_Name;
        Ref<SharedObject> m_Library;

        FConstructor constructor = nullptr;
        FDestructor  destructor  = nullptr;
        FOnCreate    on_create   = nullptr;
        FOnUpdate    on_update   = nullptr;
        FOnDestroy   on_destroy  = nullptr;
        FAlignOf     align_of    = nullptr;
        FSizeOf      size_of     = nullptr;
    };
}  // namespace DE
