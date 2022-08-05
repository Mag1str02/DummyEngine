#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

Ref<Scene> r_scene = CreateRef<Scene>("BasicScene");
Scene& scene = *r_scene;

void FillScene() {}

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
        scene["flashlight"].GetComponent<LightSource>().position = scene["player"].GetComponent<FPSCamera>().GetPos();
        scene["flashlight"].GetComponent<LightSource>().direction = scene["player"].GetComponent<FPSCamera>().GetDir();

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
                // std::cout << "Found entity with pos" << std::endl;
            }
        }
        // std::cout << "-------------------" << std::endl;
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
        Initialize();
        RegisterSystems();
        CreateEntities();
        LoadShaders();
        LoadModels();
        SetObjectProperties();
    }
    virtual void OnClose() override
    {
        SceneLoader::Save(r_scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / (r_scene->GetName() + ".yml"));
    }
    virtual void Update(double dt) override
    {
        ProcessInput(dt);
        scene.OnUpdate(dt);
        scene.Render();
    }

private:
    void Initialize()
    {
        Logger::Stage("loading", "Main", "INITIALIZETION");
        Input::SetWindow(m_Window->GetWindow());
        Renderer::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Renderer::Disable(RenderSetting::FaceCulling);
    }
    void RegisterSystems()
    {
        scene.RegisterSystem<MovingSystem>();
        scene.RegisterSystem<LightManager>();
    }

    void CreateEntities()
    {
        // Creating entities
        {
            auto player = scene.CreateEntity("player");
            auto sponza = scene.CreateEntity("sponza");
            auto train = scene.CreateEntity("train");
            auto sun = scene.CreateEntity("sun");
            auto moon = scene.CreateEntity("moon");
            auto lamp_white = scene.CreateEntity("lamp_white");
            auto lamp_magenta = scene.CreateEntity("lamp_magenta");
            auto flashlight = scene.CreateEntity("flashlight");
            auto lamp = scene.CreateEntity("lamp");
            auto surface = scene.CreateEntity("surface");
            auto colored_phong = scene.CreateEntity("ColoredPhong");
            auto textured_phong = scene.CreateEntity("TexturedPhong");
            auto backpack = scene.CreateEntity("backpack");
        }
        // Adding componenets
        {
            scene["player"].AddComponent<FPSCamera>();

            scene["backpack"].AddComponent<Transformation>();
            // scene["backpack"].AddComponent<LinearManipulator>();

            scene["sponza"].AddComponent<Transformation>();
            scene["sponza"].AddComponent<Ref<Shader>>();

            scene["train"].AddComponent<Transformation>();
            scene["lamp_white"].AddComponent<Transformation>();
            scene["lamp_magenta"].AddComponent<Transformation>();
            scene["lamp"].AddComponent<Transformation>();
            scene["surface"].AddComponent<Transformation>();

            scene["train"].AddComponent<Ref<Shader>>();
            scene["backpack"].AddComponent<Ref<Shader>>();

            // scene["backpack"].AddComponent<LinearManipulator>();

            scene["sun"].AddComponent<LightSource>();
            scene["moon"].AddComponent<LightSource>();
            scene["lamp_white"].AddComponent<LightSource>();
            scene["lamp_magenta"].AddComponent<LightSource>();
            scene["flashlight"].AddComponent<LightSource>();
            scene["lamp"].AddComponent<LightSource>();

            // scene["lamp_white"].AddComponent<LinearManipulator>();
            // scene["lamp_magenta"].AddComponent<LinearManipulator>();

            scene["ColoredPhong"].AddComponent<UniqueShader>();
            scene["TexturedPhong"].AddComponent<UniqueShader>();
        }
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

        scene["ColoredPhong"].GetComponent<UniqueShader>().shader = colored_phong;
        scene["TexturedPhong"].GetComponent<UniqueShader>().shader = textured_phong;

        scene["train"].AddComponent<Ref<Shader>>(colored_phong);
        scene["surface"].AddComponent<Ref<Shader>>(colored_phong);
        scene["backpack"].AddComponent<Ref<Shader>>(textured_phong);
        scene["sponza"].AddComponent<Ref<Shader>>(textured_phong);

        Logger::Info("loading", "Main", "Shaders loaded.");
    }

    void LoadModels()
    {
        Logger::Stage("loading", "Main", "LOADING MODELS");

        Ref<RenderModelData> train, backpack, cube, cubes, sponza;
        RenderModel r_train, r_backpack, r_cube, r_cubes, r_sponza;

        train = ModelLoader::Load(Config::GetPath(DE_CFG_MODEL_PATH) / "Train" / "train.obj");
        backpack = ModelLoader::Load(Config::GetPath(DE_CFG_MODEL_PATH) / "Backpack" / "backpack.obj");
        cube = ModelLoader::Load(Config::GetPath(DE_CFG_MODEL_PATH) / "Cube" / "cube.obj");
        cubes = ModelLoader::Load(Config::GetPath(DE_CFG_MODEL_PATH) / "Cubes" / "cubes.obj");
        sponza = ModelLoader::Load(Config::GetPath(DE_CFG_MODEL_PATH) / "Sponza" / "sponza.obj");

        Logger::Stage("loading", "Main", "Models Loaded");

        train->Compress();
        backpack->Compress();
        cubes->Compress();
        cube->Compress();

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
        scene["sponza"].AddComponent<RenderModel>(r_sponza);

        scene["lamp_white"].AddComponent<RenderModel>(r_cube);
        scene["lamp_magenta"].AddComponent<RenderModel>(r_cube);

        scene["lamp_white"].AddComponent<Ref<Shader>>(scene["ColoredPhong"].GetComponent<UniqueShader>().shader);
        scene["lamp_magenta"].AddComponent<Ref<Shader>>(scene["ColoredPhong"].GetComponent<UniqueShader>().shader);
    }
    void SetObjectProperties()
    {
        Logger::Stage("loading", "Main", "SETTING OBJECTS PROPERTIES");

        scene["sponza"].GetComponent<Transformation>().translation = Vec3(0, 50, 0);
        scene["sponza"].GetComponent<Transformation>().scale = Vec3(0.01);

        scene["backpack"].GetComponent<Transformation>().translation = Vec3(30, 0, 0);
        // scene["backpack"].GetComponent<LinearManipulator>().radius = 0;

        scene["surface"].GetComponent<Transformation>().translation = Vec3(0, -10, 0);
        scene["surface"].GetComponent<Transformation>().scale = Vec3(500, 0.1, 500);

        FPSCamera& camera = scene["player"].GetComponent<FPSCamera>();

        LightSource& sun = scene["sun"].GetComponent<LightSource>();
        LightSource& moon = scene["moon"].GetComponent<LightSource>();
        LightSource& lamp_white = scene["lamp_white"].GetComponent<LightSource>();
        LightSource& lamp_magenta = scene["lamp_magenta"].GetComponent<LightSource>();
        LightSource& flashlight = scene["flashlight"].GetComponent<LightSource>();
        LightSource& lamp = scene["lamp"].GetComponent<LightSource>();

        sun.type = LightSourceType::Direction;
        moon.type = LightSourceType::Direction;
        lamp_white.type = LightSourceType::Point;
        lamp_magenta.type = LightSourceType::Point;
        flashlight.type = LightSourceType::Spot;
        lamp.type = LightSourceType::Spot;

        scene["lamp_white"].GetComponent<Transformation>().translation = Vec3(20, 20, 20);
        scene["lamp_magenta"].GetComponent<Transformation>().translation = Vec3(-10, 10, -30);
        scene["lamp"].GetComponent<Transformation>().translation = Vec3(30, 10, 0);

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
