#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiManager.h"
#include "DummyEditor/Panels/InspectorPanel.h"
#include "DummyEditor/Panels/MenuBar.h"
#include "DummyEditor/Panels/ProfilerPanel.h"
#include "DummyEditor/Panels/RendererPanel.h"
#include "DummyEditor/Panels/SceneHierarchyPanel.h"
#include "DummyEditor/Panels/ThemePanel.h"
#include "DummyEditor/Panels/ViewportPanel.h"

namespace DE {

    class TransformSyncSystem : public System {
    public:
        TransformSyncSystem() = default;
        virtual std::string GetName() const override { return "TransformSyncSystem"; }
        void                Update(float dt) override;
    };
    class AnimationSystem : public System {
    public:
        AnimationSystem() = default;
        virtual std::string GetName() const override { return "AnimationSystem"; }
        void                Update(float dt) override;
    };

    class EditorLayer : public DE::Layer {
    public:
        enum class SceneState {
            None = 0,
            Paused,
            Running,
            Step,
            Editing,
        };
        enum class SceneScriptState {
            Uncompiled,
            Compiled,
            OldCompiled,
        };
        enum class InputState { NonSpecified = 0, ViewPort };
        struct Resources {
            Ref<Texture> play_icon;
            Ref<Texture> pause_icon;
            Ref<Texture> step_icon;
            Ref<Texture> stop_icon;
            Ref<Texture> build_icon;
            Ref<Texture> build_and_run_icon;
        };

        //*___Layer____________________________________________________________________________________________________________________________________________________________________________________

        virtual void OnAttach() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnImGuiRender() override;
        virtual void OnDetach() override;

        //*___EditorActions____________________________________________________________________________________________________________________________________________________________________________

        void ActionOpenScene();
        void ActionCloseScene();
        void ActionSaveScene();
        void ActionCreateScene();
        void ActionBuild();
        void ActionBuildAndRun();
        void ActionRunScene();
        void ActionStopScene();
        void ActionPauseScene();
        void ActionResumeScene();
        void ActionStepScene();

        //*___Editor___________________________________________________________________________________________________________________________________________________________________________________

        EditorLayer();
        ~EditorLayer();
        static EditorLayer& Get();

        ImGuiManager&    GetImGuiManager();
        SceneScriptState GetSceneScriptState() const;
        SceneState       GetSceneState() const;
        InputState       GetInputState() const;
        Resources&       GetResources();
        Ref<Scene>       GetScene() const;

    private:
        //*___Helpers__________________________________________________________________________________________________________________________________________________________________________________

        void ReloadScripts();

        Path OpenSceneDialog();
        Path SaveSceneDialog();
        void OpenScene(const Path& path);
        void SaveScene(const Path& path);
        void CloseScene();
        void RunScene();
        void StopScene();

        void LoadAssets();
        void UnloadAssets();
        void PrepareScene();

        void ProcessControlls(float dt);

        void LoadEditorResources();
        void LoadIcons();

        SceneScriptState m_SceneScriptState = SceneScriptState::Uncompiled;
        InputState       m_InputState       = InputState::NonSpecified;
        SceneState       m_SceneState       = SceneState::None;

        ImGuiManager        m_ImGuiManager;
        ViewportPanel       m_Viewport;
        ViewportPanel       m_FBOViewport;
        SceneHierarchyPanel m_SceneHierarchy;
        InspectorPanel      m_Inspector;
        ProfilerPanel       m_Profiler;
        ThemePanel          m_ThemePanel;
        MenuBar             m_MenuBar;
        RendererPanel       m_RendererPanel;

        Ref<Scene>               m_CurrentScene;
        SceneFileData            m_SceneFileData;
        Entity                   m_EditorCamera;
        Ref<TransformSyncSystem> m_TSSystem;

        Resources           m_Resources;
        static EditorLayer* s_Instance;
    };
}  // namespace DE