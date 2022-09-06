#pragma once

#include "DummyEngineInclude.h"
#include "Panels/ViewportPanel.h"
#include "Panels/ProfilerPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"

namespace DE
{
    struct SceneData
    {
        Ref<Scene> scene;
        Ref<FrameBuffer> frame_buffer;
    };

    struct EditorState
    {
        bool m_ViewportEnabled = true;
        bool m_ProfilerEnabled = true;
        bool m_InspectorEnabled = true;
        bool m_DemoWindowEnabled = true;
        bool m_SceneHierarchyEnabled = true;
    };

    class EditorLayer : public DE::Layer
    {
    public:
        EditorLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnImGuiRender() override;

    private:
        //*~~~EditorGUI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void ShowDockingSpace();
        void ShowDockingSpaceTabBar();

        //*~~~EditorFunctionality~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void OpenSceneDialog();
        void SaveSceneDialog();
        void OpenScene(const Path& path);
        void SaveScene(const Path& path);

        void ProcessControlls();

        ViewportPanel m_Viewport;
        SceneHierarchyPanel m_SceneHierarchy;
        InspectorPanel m_Inspector;
        ProfilerPanel m_Profiler;
        EditorState m_State;
        SceneData m_SceneData;
    };
}  // namespace DE