#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiManager.h"
#include "DummyEditor/Panels/InspectorPanel.h"
#include "DummyEditor/Panels/MenuBar.h"
#include "DummyEditor/Panels/ProfilerPanel.h"
#include "DummyEditor/Panels/SceneHierarchyPanel.h"
#include "DummyEditor/Panels/ThemePanel.h"
#include "DummyEditor/Panels/ViewportPanel.h"

namespace DE {
    struct SceneData {
        Ref<Scene>       scene;
        SceneFileData    file_data;
    };
    enum class InputState { NonSpecified = 0, ViewPort };

    struct EditorState {
        bool m_ViewportEnabled       = true;
        bool m_ProfilerEnabled       = true;
        bool m_InspectorEnabled      = true;
        bool m_SceneHierarchyEnabled = true;

        InputState m_InputState;
    };

    class EditorLayer : public DE::Layer {
    public:
        EditorLayer();
        ~EditorLayer();

        static EditorLayer& Get();

        virtual void OnAttach() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnImGuiRender() override;
        virtual void OnDetach() override;

        //*___EditorActions____________________________________________________________________________________________________________________________________________________________________________

        void ActionOpenScene();
        void ActionCloseScene();
        void ActionSaveScene();
        void ActionCreateScene();
        void ActionReloadScripts();

    private:
        friend class MenuBar;

        //*___Helpers__________________________________________________________________________________________________________________________________________________________________________________

        Path OpenSceneDialog();
        Path SaveSceneDialog();

        void OpenScene(const Path& path);
        void SaveScene(const Path& path);
        void ReloadScripts();
        void CloseScene();

        void LoadAssets();
        void UnloadAssets();
        void PrepareScene();

        void ProcessControlls(float dt);

        ViewportPanel       m_Viewport;
        SceneHierarchyPanel m_SceneHierarchy;
        InspectorPanel      m_Inspector;
        ProfilerPanel       m_Profiler;
        ThemePanel          m_ThemePanel;
        MenuBar             m_MenuBar;

        ImGuiManager m_ImGuiManager;

        EditorState m_State;
        Entity      m_EditorCamera;

        SceneData m_SceneData;

        static EditorLayer* s_Instance;
    };
}  // namespace DE