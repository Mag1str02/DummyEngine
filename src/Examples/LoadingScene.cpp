#include "../DummyEngine/Addition/DummyEngine.h"
#include "../DummyEngine/Core/Application/EntryPoint.h"

using namespace DE;

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

class MovingSystem : public System
{
public:
    MovingSystem() {}
    virtual std::string GetName() const override { return "MovingSystem"; }

    void Update(double dt) override
    {
        auto& positions     = GetComponentArray<TransformComponent>();
        auto& meshes        = GetComponentArray<RenderMeshComponent>();
        auto& light_sources = GetComponentArray<LightSource>();

        for (auto [id, mesh] : meshes)
        {
            mesh.mesh_instance->at<Mat4>(0) = positions[id].GetTransform();
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
    MyLayer() : Layer("LoadingScene")
    {
        m_EventDispatcher.AddEventListener<WindowResizeEvent>(
            [this](WindowResizeEvent& event)
            {
                FPSCamera& camera = scene->GetByName("player").GetComponent<FPSCamera>();
                if (event.GetWidth() != 0 && event.GetHeight() != 0)
                {
                    camera.SetAspect(double(event.GetWidth()) / event.GetHeight());
                }
            });
    }

    virtual void OnAttach() override
    {
        Logger::Stage("loading", "Main", "INITIALIZETION");
        Renderer::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        scene = CreateRef<Scene>("NewScene");
        SceneLoader::Load(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Grass.yml");
        scene->RegisterSystem<MovingSystem>();

        GenGrass();
    }
    virtual void OnUpdate(float dt) override
    {
        ProcessInput(dt);
        scene->OnUpdate(dt);
        scene->Render();
        ResourceManager::GetResource<Shader>(3566111628970928214)->Bind();
        ResourceManager::GetResource<Shader>(3566111628970928214)->SetFloat("u_Time", glfwGetTime());
        Renderer::GetRenderAPI().DrawArrays(grass, RenderPrimitive::Point);
        glCheckError();
    }
    virtual void OnDetach() override
    {
        // SceneLoader::Save(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
    }
    virtual void OnEvent(Event& event) override { m_EventDispatcher.Dispatch(event); }

private:
    void ProcessInput(float dt)
    {
        float sensitivity = 0.07;
        float speed       = 15;

        if (Input::KeyReleased(GLFW_KEY_TAB))
        {
            SetMouseLockToggleEvent event;
            BroadcastEvent(event);
        }

        if (Input::KeyReleased(GLFW_KEY_F11))
        {
            if (!windowed)
            {
                SetWindowModeWindowedEvent event;
                BroadcastEvent(event);
            }
            else
            {
                int monitor_id = 0;
                if (Input::KeyDown(GLFW_KEY_1))
                {
                    monitor_id = 1;
                }

                SetWindowModeFullscreenEvent event(monitor_id);
                BroadcastEvent(event);
            }
            windowed = !windowed;
        }

        if (!Input::MouseLocked())
        {
            return;
        }

        FPSCamera& camera = scene->GetByName("player").GetComponent<FPSCamera>();
        camera.RotateY(Input::CursorXOffset() * sensitivity);
        camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

        if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            speed = 100.0f;
        }
        if (Input::KeyDown(GLFW_KEY_ESCAPE))
        {
            WindowCloseEvent event;
            BroadcastEvent(event);
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

    void GenGrass()
    {
        uint32_t x_amount    = 500;
        uint32_t y_amount    = 500;
        auto     interpolate = [](float min, float max, int index, int max_index) { return min + (max - min) * (float(index) / (max_index - 1)); };

        std::vector<Vec3> data(x_amount * y_amount * 3);
        for (size_t i = 0; i < x_amount * y_amount; ++i)
        {
            int x           = i % x_amount;
            int y           = i / y_amount - 1;
            data[3 * i]     = Vec3(interpolate(-100, 100, x, x_amount), -99, interpolate(-100, 100, y, y_amount));
            data[3 * i + 1] = Vec3(0, 1, 0);
            data[3 * i + 2] = glm::rotate(Vec3(1.0, 0, 0), Random::Float(0, M_PI * 2), Vec3(0, 1, 0));
        }

        grass   = VertexArray::Create();
        auto bf = VertexBuffer::Create({BufferElementType::Float3, BufferElementType::Float3, BufferElementType::Float3}, y_amount * x_amount, &data[0]);
        grass->AddVertexBuffer(bf);
    }

    EventDispatcher  m_EventDispatcher;
    Ref<Scene>       scene;
    bool             windowed = true;
    Ref<VertexArray> grass;
};

namespace DE
{
    Application* CreateApplication()
    {
        Application* app = new Application("MyApplication");
        app->PushLayer(new MyLayer());
        return app;
    }
}  // namespace DE
