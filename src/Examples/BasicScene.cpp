#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

using Scene = std::unordered_map<std::string, Entity>;
Scene scene;

struct Drawable
{};

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
        return Vec3(((max_scale + min_scale) / 2) +
                    ((max_scale - min_scale) / 2) * (std::sin(time_offset + glfwGetTime())));
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
        scene["flashlight"].GetComponent<SpotLight>().position = scene["player"].GetComponent<FPSCamera>().GetPos();
        scene["flashlight"].GetComponent<SpotLight>().direction = scene["player"].GetComponent<FPSCamera>().GetDir();

        auto& manipulators = GetComponentArray<LinearManipulator>();
        auto& positions = GetComponentArray<Transformation>();
        auto& scales = GetComponentArray<ScaleManipulator>();
        auto& point_lights = GetComponentArray<PointLight>();

        for (auto [entity_id, linear_manipulator] : manipulators)
        {
            positions[entity_id].translation += linear_manipulator.Update(dt);
        }
        for (auto [entity_id, scale] : scales)
        {
            positions[entity_id].scale = scale.GetScale();
        }
        for (auto [entity_id, point_light] : point_lights)
        {
            point_light.position = positions[entity_id].translation;
        }
    }
};
class DrawSystem : public System
{
public:
    DrawSystem() {}
    virtual std::string GetName() const override
    {
        return "DrawSystem";
    }
    void Update(double dt) override
    {
        auto& drawables = GetComponentArray<Drawable>();
        auto& models = GetComponentArray<RenderModel>();
        auto& shaders = GetComponentArray<Ref<Shader>>();
        auto& transformations = GetComponentArray<Transformation>();

        Renderer::Clear();

        auto& camera = scene["player"].GetComponent<FPSCamera>();
        for (auto [entity_id, shader] : shaders)
        {
            shader->Bind();
            shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
            shader->SetFloat3("u_CameraPos", camera.GetPos());
        }
        for (auto [entity_id, drawable] : drawables)
        {
            Renderer::Submit(shaders[entity_id], models[entity_id], transformations[entity_id].GetTransform());
        }
    }
};

class TestApplication : public Application
{
public:
    TestApplication(std::string name) : Application(name)
    {
        cursor_mode = true;
    }

    virtual void OnLoad() override
    {
        Initialize();
        RegisterSystems();
        CreateEntities();
        LoadShaders();
        LoadModels();
        SetObjectProperties();

        ComponentManager::Get().LogState();
        EntityManager::Get().LogState();
    }
    virtual void Update(double dt) override
    {
        ProcessInput(dt);
    }

private:
    void Initialize()
    {
        Logger::Stage("loading", "Main", "INITIALIZETION");
        Input::SetWindow(m_Window->GetWindow());
        Renderer::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }
    void RegisterSystems()
    {
        SystemManager::RegisterSystem<MovingSystem>();
        SystemManager::RegisterSystem<LightManager>();
        SystemManager::RegisterSystem<DrawSystem>();

        SystemManager::AddOrder<MovingSystem, LightManager>();
        SystemManager::AddOrder<LightManager, DrawSystem>();
        SystemManager::CalculateOrder();
    }

    void CreateEntities()
    {
        scene["player"] = Entity();
        scene["backpack"] = Entity();
        scene["sponza"] = Entity();
        scene["train"] = Entity();
        scene["sun"] = Entity();
        scene["moon"] = Entity();
        scene["lamp_white"] = Entity();
        scene["lamp_magenta"] = Entity();
        scene["flashlight"] = Entity();
        scene["lamp"] = Entity();
        scene["surface"] = Entity();
        scene["colored_phong"] = Entity();
        scene["textured_phong"] = Entity();

        scene["player"].AddComponent<FPSCamera>();

        scene["backpack"].AddComponent<Transformation>();

        scene["sponza"].AddComponent<Transformation>();
        scene["sponza"].AddComponent<Drawable>();
        scene["sponza"].AddComponent<Ref<Shader>>();

        scene["train"].AddComponent<Transformation>();
        scene["lamp_white"].AddComponent<Transformation>();
        scene["lamp_magenta"].AddComponent<Transformation>();
        scene["lamp"].AddComponent<Transformation>();
        scene["flashlight"].AddComponent<Transformation>();
        scene["surface"].AddComponent<Transformation>();

        scene["backpack"].AddComponent<Drawable>();
        scene["train"].AddComponent<Drawable>();
        scene["surface"].AddComponent<Drawable>();

        scene["train"].AddComponent<Ref<Shader>>();
        scene["backpack"].AddComponent<Ref<Shader>>();

        scene["backpack"].AddComponent<LinearManipulator>();

        scene["sun"].AddComponent<DirectionalLight>();
        scene["moon"].AddComponent<DirectionalLight>();
        scene["lamp_white"].AddComponent<PointLight>();
        scene["lamp_magenta"].AddComponent<PointLight>();
        scene["flashlight"].AddComponent<SpotLight>();
        scene["lamp"].AddComponent<SpotLight>();
    }
    void LoadShaders()
    {
        Logger::Stage("loading", "Main", "LOADING SHADERS");

        Ref<Shader> colored_phong =
            Shader::Create("ColoredPhong",
                           {{.type = ShaderPartType::Vertex,
                             .path = Config::GetPath(DE_CFG_SHADER_PATH) / "ColoredPhong" / "ColoredPhong.vs"},
                            {.type = ShaderPartType::Fragment,
                             .path = Config::GetPath(DE_CFG_SHADER_PATH) / "ColoredPhong" / "ColoredPhong.fs"}});
        Ref<Shader> textured_phong =
            Shader::Create("TexturedPhong",
                           {{.type = ShaderPartType::Vertex,
                             .path = Config::GetPath(DE_CFG_SHADER_PATH) / "TexturedPhong" / "TexturedPhong.vs"},
                            {.type = ShaderPartType::Fragment,
                             .path = Config::GetPath(DE_CFG_SHADER_PATH) / "TexturedPhong" / "TexturedPhong.fs"}});

        scene["colored_phong"].GetComponent<UniqueShader>().shader = colored_phong;
        scene["textured_phong"].GetComponent<UniqueShader>().shader = textured_phong;
        scene["train"].AddComponent<Ref<Shader>>(colored_phong);
        scene["surface"].AddComponent<Ref<Shader>>(colored_phong);
        scene["backpack"].AddComponent<Ref<Shader>>(textured_phong);
        scene["sponza"].AddComponent<Ref<Shader>>(textured_phong);

        Logger::Info("loading", "Main", "Shaders loaded.");
    }

    void LoadModels()
    {
        Logger::Stage("loading", "Main", "LOADING MODELS");

        RenderModelData train, backpack, cube, cubes, sponza;
        RenderModel r_train, r_backpack, r_cube, r_cubes, r_sponza;

        ModelLoader::LoadModel(Config::GetPath(DE_CFG_MODEL_PATH) / "Train" / "train.obj", train);
        ModelLoader::LoadModel(Config::GetPath(DE_CFG_MODEL_PATH) / "Backpack" / "backpack.obj", backpack);
        ModelLoader::LoadModel(Config::GetPath(DE_CFG_MODEL_PATH) / "Cube" / "cube.obj", cube);
        ModelLoader::LoadModel(Config::GetPath(DE_CFG_MODEL_PATH) / "Cubes" / "cubes.obj", cubes);
        ModelLoader::LoadModel(Config::GetPath(DE_CFG_MODEL_PATH) / "Sponza" / "sponza.obj", sponza);

        Logger::Stage("loading", "Main", "Models Loaded");

        train.Compress();
        backpack.Compress();
        cubes.Compress();
        cube.Compress();

        Logger::Stage("loading", "Main", "Compressed");

        r_sponza.FillData(sponza);
        r_train.FillData(train);
        r_backpack.FillData(backpack);
        r_cube.FillData(cube);
        r_cubes.FillData(cubes);

        Logger::Stage("loading", "Main", "Data Filled");

        scene["backpack"].AddComponent<RenderModel>(r_backpack);
        scene["train"].AddComponent<RenderModel>(r_train);
        scene["surface"].AddComponent<RenderModel>(r_cubes);
        scene["cubes"].AddComponent<RenderModel>(r_cubes);
        scene["sponza"].AddComponent<RenderModel>(r_sponza);

        scene["lamp_white"].AddComponent<RenderModel>(r_cube);
        scene["lamp_magenta"].AddComponent<RenderModel>(r_cube);
        scene["lamp_white"].AddComponent<Drawable>();
        scene["lamp_magenta"].AddComponent<Drawable>();
        scene["lamp_white"].AddComponent<Ref<Shader>>(scene["colored_phong"].GetComponent<UniqueShader>().shader);
        scene["lamp_magenta"].AddComponent<Ref<Shader>>(scene["colored_phong"].GetComponent<UniqueShader>().shader);

        for (int i = 0; i < 2; ++i)
        {
            scene["train" + std::to_string(i)].AddComponent<RenderModel>(scene["train"].GetComponent<RenderModel>());
            scene["train" + std::to_string(i)].GetComponent<Transformation>().translation = Vec3(0, 100, 300 - i * 12);
            scene["train" + std::to_string(i)].AddComponent<Ref<Shader>>(scene["train"].GetComponent<Ref<Shader>>());
            scene["train" + std::to_string(i)].AddComponent<LinearManipulator>();
            scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().current_time = i * 3.14 / 16;
            scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().radius = 15;
            scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().dir =
                Vec3(std::sin(i * 3.14 / 16), std::cos(i * 3.14 / 16), 0);
            scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().speed = std::sin(i * 3.14 / 8) + 1;
            scene["train" + std::to_string(i)].AddComponent<ScaleManipulator>();
            scene["train" + std::to_string(i)].GetComponent<ScaleManipulator>().min_scale = -1;
            scene["train" + std::to_string(i)].GetComponent<ScaleManipulator>().max_scale = 1;
            scene["train" + std::to_string(i)].GetComponent<ScaleManipulator>().time_offset = i * 3.14 / 16;
            scene["train" + std::to_string(i)].AddComponent<Drawable>();
        }
    }
    void SetObjectProperties()
    {
        Logger::Stage("loading", "Main", "SETTING OBJECTS PROPERTIES");

        scene["sponza"].GetComponent<Transformation>().translation = Vec3(0, 50, 0);
        scene["sponza"].GetComponent<Transformation>().scale = Vec3(0.01);

        scene["backpack"].GetComponent<Transformation>().translation = Vec3(30, 0, 0);
        scene["backpack"].GetComponent<LinearManipulator>().radius = 0;

        scene["surface"].GetComponent<Transformation>().translation = Vec3(0, -10, 0);
        scene["surface"].GetComponent<Transformation>().scale = Vec3(500, 0.1, 500);

        FPSCamera& camera = scene["player"].GetComponent<FPSCamera>();
        DirectionalLight& sun = scene["sun"].GetComponent<DirectionalLight>();
        DirectionalLight& moon = scene["moon"].GetComponent<DirectionalLight>();
        PointLight& lamp_white = scene["lamp_white"].GetComponent<PointLight>();
        PointLight& lamp_magenta = scene["lamp_magenta"].GetComponent<PointLight>();
        SpotLight& flashlight = scene["flashlight"].GetComponent<SpotLight>();
        SpotLight& lamp = scene["lamp"].GetComponent<SpotLight>();

        scene["lamp_white"].GetComponent<LinearManipulator>().dir = Vec3(1, 0, 0);
        scene["lamp_white"].GetComponent<LinearManipulator>().radius = 50;
        scene["lamp_white"].GetComponent<LinearManipulator>().speed = 0.5;
        scene["lamp_white"].GetComponent<Transformation>().translation = Vec3(0, 20, 0);

        scene["lamp_magenta"].GetComponent<LinearManipulator>().dir = Vec3(0, 0, 1);
        scene["lamp_magenta"].GetComponent<LinearManipulator>().radius = 50;
        scene["lamp_magenta"].GetComponent<LinearManipulator>().speed = 0.5;
        scene["lamp_magenta"].GetComponent<Transformation>().translation = Vec3(0, 10, 0);

        camera.SetPos(Vec3(0.0f, 0.0f, 10.0f));

        moon.ambient = Vec3(0.2f, 0.56f, 1.0f) * 0.02f;
        moon.diffuse = Vec3(0.2f, 0.56f, 1.0f) * 0.3f;
        moon.specular = Vec3(0.2f, 0.56f, 1.0f) * 0.6f;
        moon.direction = glm::normalize(Vec3(0, -1, 2));

        sun.ambient = Vec3(1.0f, 0.73f, 0.2f) * 0.02f;
        sun.diffuse = Vec3(1.0f, 0.73f, 0.2f) * 0.6f;
        sun.specular = Vec3(1.0f, 0.73f, 0.2f) * 0.3f;
        sun.direction = glm::normalize(Vec3(0, 1, -2));

        lamp_white.ambient = COLOR_WHITE * 0.1f;
        lamp_white.diffuse = COLOR_WHITE * 0.3f;
        lamp_white.specular = COLOR_WHITE * 0.6f;
        lamp_white.clq = Vec3(1.0f, 0.007f, 0.0002f);

        lamp_magenta.ambient = COLOR_MAGENTA * 0.1f;
        lamp_magenta.diffuse = COLOR_MAGENTA * 0.6f;
        lamp_magenta.specular = COLOR_MAGENTA * 0.3f;
        lamp_magenta.clq = Vec3(1.0f, 0.022f, 0.0019f);

        flashlight.ambient = COLOR_WHITE * 0.1f;
        flashlight.diffuse = COLOR_WHITE * 0.4f;
        flashlight.specular = COLOR_WHITE * 0.4f;
        flashlight.clq = Vec3(1.0f, 0.007f, 0.0002f);
        flashlight.inner_cone_cos = cos(glm::radians(12.5));
        flashlight.outer_cone_cos = cos(glm::radians(20.5));

        lamp.ambient = COLOR_RED * 0.1f;
        lamp.diffuse = COLOR_RED * 0.6f;
        lamp.specular = COLOR_RED * 0.4f;
        lamp.clq = Vec3(1.0f, 0.022f, 0.0019f);
        lamp.position = Vec3(0.0f, 50.0f, 0.0f);
        lamp.direction = Vec3(0.0f, -1.0f, 0.0f);
        lamp.inner_cone_cos = cos(glm::radians(12.5));
        lamp.outer_cone_cos = cos(glm::radians(17.5));
    }

    void ProcessInput(float dt)
    {
        FPSCamera& camera = scene["player"].GetComponent<FPSCamera>();
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
            speed = 50.0f;
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
