#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

Ref<Scene> scene;

class MovingSystem : public System
{
public:
    MovingSystem() {}
    virtual std::string GetName() const override
    {
        return "MovingSystem";
    }

    void Update(double dt) override
    {
        auto& positions = GetComponentArray<TransformComponent>();
        auto& meshes = GetComponentArray<RenderMeshComponent>();
        auto& light_sources = GetComponentArray<LightSource>();

        for (auto [id, transformation] : positions)
        {
            meshes[id].mesh_instance->at<Mat4>(0) = transformation.GetTransform();
        }

        for (auto [entity_id, light_source] : light_sources)
        {
            if (positions.HasComponent(entity_id))
            {
                light_source.position = positions[entity_id].translation;
            }
        }
    }
};

class MyLayer : public Layer
{
public:
    TestApplication(std::string name) : Application(name)
    {
        cursor_mode = false;
    }

    virtual void OnLoad() override
    {
        Logger::Stage("loading", "Main", "INITIALIZETION");
        Input::SetWindow(m_Window->GetWindow());
        Renderer::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        scene = CreateRef<Scene>("NewScene");
        SceneLoader::Load(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
        scene->RegisterSystem<MovingSystem>();

        m_FrameBuffer = FrameBuffer::Create({1920, 1080});
        m_FrameBuffer->AddColorAttachment(TextureFormat::RGBA);
        m_FrameBuffer->SetDepthAttachment(TextureFormat::DepthStencil);

        m_ScreenShader =
            Shader::Create({{ShaderPartType::Vertex, Config::GetPath(DE_CFG_ASSET_PATH) / "Shaders" / "FrameBuffer" / "FrameBuffer.vs"},
                            {ShaderPartType::Fragment, Config::GetPath(DE_CFG_ASSET_PATH) / "Shaders" / "FrameBuffer" / "FrameBuffer.fs"}});
    }
    virtual void Update(double dt) override
    {
        DE_PROFILE_SCOPE("Processing Input", ProcessInput(dt));
        DE_PROFILE_SCOPE("Scene Update", scene->OnUpdate(dt));
        DE_PROFILE_SCOPE("Scene Render", scene->Render());
    }
    virtual void OnClose() override
    {
        // SceneLoader::Save(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
    }

private:
    void ProcessInput(float dt)
    {
        FPSCamera& camera = scene->GetByName("player").GetComponent<FPSCamera>();
        float sensitivity = 0.07;
        float speed = 15;

        Input::ReadFrame();

        if (Input::KeyReleased(GLFW_KEY_TAB))
        {
            if (cursor_mode)
            {
                glfwSetInputMode(m_Window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursor_mode = false;
            }
            else
            {
                glfwSetInputMode(m_Window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cursor_mode = true;
            }
        }

        if (Input::KeyReleased(GLFW_KEY_F11))
        {
            if (m_Window->GetState().window_mode == WindowMode::fullscreen)
            {
                m_Window->Windowed();
            }
            else
            {
                int monitor_id = 0;
                if (Input::KeyDown(GLFW_KEY_1))
                {
                    monitor_id = 1;
                }
                m_Window->FullScreen(monitor_id);
            }
            Input::SetWindow(m_Window->GetWindow());
        }

        int width, height;
        glfwGetWindowSize(m_Window->GetWindow(), &width, &height);
        if (height != 0 && width != 0)
        {
            camera.SetAspect(double(width) / height);
        }
        if (!cursor_mode)
        {
            return;
        }

        camera.RotateY(Input::CursorXOffset() * sensitivity);
        camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

        if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            speed = 100.0f;
        }
        if (Input::KeyDown(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(m_Window->GetWindow(), true);
        }
        if (Input::KeyDown(GLFW_KEY_S))
        {
            camera.MoveInLocal(Vec3(0.0f, 0.0f, -1.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_W))
        {
            camera.MoveInLocal(Vec3(0.0f, 0.0f, 1.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_D))
        {
            camera.MoveInLocal(Vec3(1.0f, 0.0f, 0.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_A))
        {
            camera.MoveInLocal(Vec3(-1.0f, 0.0f, 0.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_SPACE))
        {
            camera.MoveInWorld(Vec3(0.0f, 1.0f, 0.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_C))
        {
            camera.MoveInWorld(Vec3(0.0f, -1.0f, 0.0f) * speed * dt);
        }
    }

    Ref<Shader> m_ScreenShader;
    Ref<FrameBuffer> m_FrameBuffer;
    bool cursor_mode;
};
