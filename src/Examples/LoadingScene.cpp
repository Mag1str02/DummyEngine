#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

Ref<Scene> scene;

struct LinearManipulator
{
    Vec3 dir;
    double radius;
    double current_time;
    double speed;

    LinearManipulator() :
        dir(0, 0, 1),
        radius(10),
        current_time(0),
        speed(1){

        };
    Vec3 Update(double dt)
    {
        current_time += dt;
        return dir * float((std::sin(current_time * speed) - std::sin((current_time - dt) * speed)) * radius);
    }
};
struct ScaleManipulator
{
    double min_scale, max_scale;
    double time_offset;

    ScaleManipulator() : min_scale(0), max_scale(2), time_offset(0) {}
    Vec3 GetScale()
    {
        return Vec3(((max_scale + min_scale) / 2) + ((max_scale - min_scale) / 2) * (std::sin(time_offset + glfwGetTime())));
    }
};

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
        auto& manipulators = GetComponentArray<LinearManipulator>();
        auto& positions = GetComponentArray<Transformation>();
        auto& scales = GetComponentArray<ScaleManipulator>();
        auto& light_sources = GetComponentArray<LightSource>();

        for (auto [entity_id, linear_manipulator] : manipulators)
        {
            positions[entity_id].translation += linear_manipulator.Update(dt);
        }
        for (auto [entity_id, scale] : scales)
        {
            positions[entity_id].scale = scale.GetScale();
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

class TestApplication : public Application
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
        try
        {
            SceneLoader::Load(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
        } catch (const std::exception& e)
        {
            std::cout << "Error while loading scene: " << e.what() << std::endl;
        }

        scene->RegisterSystem<MovingSystem>();
        scene->RegisterSystem<LightManager>();
    }
    virtual void Update(double dt) override
    {
        ProcessInput(dt);
        scene->OnUpdate(dt);
        scene->Render();
    }
    virtual void OnClose() override
    {
        SceneLoader::Save(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
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

    bool cursor_mode;
};

int main()
{
    TestApplication application("Test Application");
    application.Start();
    return 0;
}
