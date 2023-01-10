#pragma once

#include "DummyEngine/Addition/Base.h"

class GLFWwindow;

namespace DE
{

    class Context
    {
    public:
        virtual ~Context() = default;

        virtual void Load() = 0;
        virtual void SwapBuffers() = 0;

        static Scope<Context> Create(GLFWwindow* window);
    };

}  // namespace DE