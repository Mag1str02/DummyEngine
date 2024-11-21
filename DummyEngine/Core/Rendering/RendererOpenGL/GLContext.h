#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Context.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class GLContext : public Context {
        LOG_AUTHOR(GLContext)
    public:
        explicit GLContext(GLFWwindow* window);

        virtual void Load() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* window_;
    };

}  // namespace DummyEngine