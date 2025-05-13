#pragma once

#include "Event.h"

#include "DummyEngine/Utils/Debug/Logger.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/PCH/DC.h"

#include <unordered_map>

struct GLFWwindow;

namespace DummyEngine {

    class GLFW : public Singleton<GLFW> {
        SINGLETON(GLFW)
        LOG_AUTHOR(GLFW)
    public:
        S_METHOD_DEF(Unit, StartEventProcessing, ());
        S_METHOD_DEF(Unit, StopEventProcessing, ());

        S_METHOD_DEF(Future<GLFWwindow*>, CreateWindow, ());
        S_METHOD_DEF(Future<Unit>, DestroyWindow, (GLFWwindow * window));
        S_METHOD_DEF(Future<Unit>, EnableFullScreen, (GLFWwindow * window, U32 monitor_id));
        S_METHOD_DEF(Future<Unit>, DisableFullScreen, (GLFWwindow * window));
        S_METHOD_DEF(Future<Unit>, SetCursorMode, (GLFWwindow * window, U32 mode));
        S_METHOD_DEF(std::vector<Event>, PullWindowEvents, (GLFWwindow * window));

    private:
        template <typename T>
        static void AddWindowEvent(GLFWwindow* window, T event);

        void InitGLFW();
        void InstallCallbacks(GLFWwindow* window);
        void OnMonitorEvent(GLFWmonitor* monitor, int event);

    private:
        FMutex                                              events_mutex_;
        std::unordered_map<GLFWwindow*, std::vector<Event>> window_events_;

        std::atomic<bool> stop_flag_ = false;
        TEvent            stopped_event_processing_;
    };
}  // namespace DummyEngine