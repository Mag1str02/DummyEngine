#pragma once

#include "Window.h"

#include "DummyEngine/Utils/Helpers/Singleton.h"

struct GLFWwindow;

namespace DummyEngine {

    class GLFW : public Singleton<GLFW> {
        SINGLETON(GLFW)
    public:
        S_METHOD_DEF(Unit, StartEventProcessing, ());
        S_METHOD_DEF(Unit, StopEventProcessing, ());

        S_METHOD_DEF(Future<GLFWwindow*>, CreateWindow, ());
        S_METHOD_DEF(Future<Unit>, DestroyWindow, (GLFWwindow * window));
        S_METHOD_DEF(Future<Unit>, EnableFullScreen, (GLFWwindow * window, U32 monitor_id));
        S_METHOD_DEF(Future<Unit>, DisableFullScreen, (GLFWwindow * window));
        S_METHOD_DEF(Future<Unit>, SetCursorMode, (GLFWwindow * window, U32 mode));

    private:
        void InitGLFW();
        void ProcessEvents();

    private:
        ThreadPool thread_pool_ = NDummyConcurrency::ThreadPool(1);

        std::atomic<bool> stop_flag_ = false;
        FEvent            stopped_event_processing_;
    };
}  // namespace DummyEngine