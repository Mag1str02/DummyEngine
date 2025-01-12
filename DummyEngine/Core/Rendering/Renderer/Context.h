#pragma once

#include "DummyEngine/Utils/Types/Types.h"

struct GLFWwindow;

namespace DummyEngine {

    class Context {
    public:
        virtual ~Context() = default;

        virtual void Load()        = 0;
        virtual void SwapBuffers() = 0;

        static Scope<Context> Create(GLFWwindow* window);
    };

}  // namespace DummyEngine