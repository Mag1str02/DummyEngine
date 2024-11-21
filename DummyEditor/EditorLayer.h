#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiManager.h"
#include "DummyEditor/Panels/InspectorPanel.h"
#include "DummyEditor/Panels/MenuBar.h"
#include "DummyEditor/Panels/ProfilerPanel.h"
#include "DummyEditor/Panels/RendererPanel.h"
#include "DummyEditor/Panels/SceneHierarchyPanel.h"
#include "DummyEditor/Panels/ThemePanel.h"
#include "DummyEditor/Panels/ViewportPanel.h"

#include "DummyEngine/Core/Application/Layer.h"
#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class Texture;

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

    class EditorLayer : public DummyEngine::Layer {
        LOG_AUTHOR(EditorLayer);

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
            Ref<Texture> PlayIcon;
            Ref<Texture> PauseIcon;
            Ref<Texture> StepIcon;
            Ref<Texture> StopIcon;
            Ref<Texture> BuildIcon;
            Ref<Texture> BuildAndRunIcon;
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

        SceneScriptState scene_script_state_ = SceneScriptState::Uncompiled;
        InputState       input_state_        = InputState::NonSpecified;
        SceneState       scene_state_        = SceneState::None;

        ImGuiManager        imgui_manager_;
        ViewportPanel       viewport_;
        ViewportPanel       shadow_viewport_;
        SceneHierarchyPanel scene_hierarchy_;
        InspectorPanel      inspector_;
        ProfilerPanel       profiler_;
        ThemePanel          theme_panel_;
        MenuBar             menu_bar_;
        RendererPanel       renderer_panel_;

        Ref<Scene>               current_scene_;
        SceneFileData            scene_file_data_;
        Entity                   editor_camera_;
        Ref<TransformSyncSystem> ts_system_;

        Resources resources_;

        static EditorLayer* gInstance;
    };
}  // namespace DummyEngine