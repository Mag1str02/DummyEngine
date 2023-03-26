#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/Panels/ViewportPanel.h"
#include "DummyEditor/Panels/ProfilerPanel.h"
#include "DummyEditor/Panels/SceneHierarchyPanel.h"
#include "DummyEditor/Panels/InspectorPanel.h"

namespace DE
{
    struct SceneData
    {
        Ref<Scene>       m_Scene;
        Ref<FrameBuffer> m_FrameBuffer;
    };
    enum class InputState
    {
        NonSpecified = 0,
        ViewPort
    };

    struct EditorState
    {
        bool m_ViewportEnabled       = true;
        bool m_ProfilerEnabled       = true;
        bool m_InspectorEnabled      = true;
        bool m_SceneHierarchyEnabled = true;

        InputState m_InputState;
    };

    class EditorLayer : public DE::Layer
    {
    public:
        EditorLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnImGuiRender() override;
        virtual void OnDetach() override;

    private:
        //*~~~EditorGUI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void ShowDockingSpace();
        void ShowDockingSpaceTabBar();

        //*~~~EditorFunctionality~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void OpenSceneDialog();
        void SaveSceneDialog();
        void OpenScene(const Path& path);
        void SaveScene(const Path& path);

        void ProcessControlls(float dt);

        ViewportPanel       m_Viewport;
        SceneHierarchyPanel m_SceneHierarchy;
        InspectorPanel      m_Inspector;
        ProfilerPanel       m_Profiler;
        ImGuiPanelStack     m_Panels;

        EditorState m_State;
        Entity      m_EditorCamera;

        SceneData m_SceneData;
    };
}  // namespace DE