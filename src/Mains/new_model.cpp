#include "../Engine/Addition/de_lib.h"

using namespace DE;

glm::mat4 e = glm::mat4(1.0f);

bool cursor_mode = true;

InputManager input_manager;
ApplicationWindow application_window;
const Model *backpack, *train, *cube, *cubes;

std::unordered_map<std::string, Entity> scene;

void Initialize();

void CreateEntities();
void RegisterSystems();
void LoadShaders();
void LoadModels();
void InitModels();
void SetObjectProperties();

void ProcessInput();
void UpdateWorld();
void Draw();

int main()
{
    Initialize();
    RegisterSystems();
    CreateEntities();
    LoadShaders();
    LoadModels();
    InitModels();
    SetObjectProperties();
    ComponentManager::Get().LogState();
    EntityManager::Get().LogState();
    Logger::Info("loading", "Main", "Loading time: " + std::to_string(glfwGetTime()) + "s");
    Logger::Close("loading");
    Logger::Open(LOG_DIR / "rendering.txt", "rendering");
    application_window.StartLoop();
    deTerminate();
    glfwTerminate();
    return 0;
}

void Initialize()
{
    std::cout << WORKING_DIR << std::endl;
    Logger::Open(LOG_DIR / "loading.txt", "loading");
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    Logger::Stage("loading", "Main", "INITIALIZETION");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    deHint(H_MAX_ENTITY_AMOUNT, 10);
    deInitialize();
    application_window.Init("First");
    application_window.SetProcessInputFunc(ProcessInput);
    application_window.SetUpdateWorldFunc(UpdateWorld);
    application_window.SetDrawFunc(Draw);
    input_manager.SetWindow(application_window.GetWindow());
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
}

void RegisterSystems()
{
    SystemManager::RegisterSystem<LightManager>();
}
void CreateEntities()
{
    scene["player"] = Entity();
    scene["backpack"] = Entity();
    scene["train"] = Entity();
    scene["sun"] = Entity();
    scene["moon"] = Entity();
    scene["lamp_white"] = Entity();
    scene["lamp_magenta"] = Entity();
    scene["flashlight"] = Entity();
    scene["lamp"] = Entity();

    scene["player"].AddComponent<FPSCamera>();

    scene["backpack"].AddComponent<Transformation>();
    scene["train"].AddComponent<Transformation>();
    scene["lamp_white"].AddComponent<Transformation>();
    scene["lamp_magenta"].AddComponent<Transformation>();
    scene["lamp"].AddComponent<Transformation>();
    scene["flashlight"].AddComponent<Transformation>();

    scene["train"].AddComponent<ShaderProgram>();
    scene["backpack"].AddComponent<ShaderProgram>();

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
    scene["train"].GetComponent<ShaderProgram>().SmartInit(SHADER_DIR / "ColoredPhong");
    scene["backpack"].GetComponent<ShaderProgram>().SmartInit(SHADER_DIR / "TexturedPhong");

    Logger::Info("loading", "Main", "Shaders loaded.");
}

void LoadModels()
{
    Logger::Stage("loading", "Main", "LOADING MODELS");
    MeshManager::AddModel("train", MODEL_DIR / "Train" / "train1.obj");
    MeshManager::AddModel("backpack", MODEL_DIR / "Backpack" / "Backpack.obj");
    MeshManager::AddModel("cube", MODEL_DIR / "Cube" / "cube.obj");
    MeshManager::AddModel("cubes", MODEL_DIR / "Cubes" / "cubes.obj");
    MeshManager::CompressModel("train");
    MeshManager::CompressModel("backpack");
    MeshManager::CompressModel("cubes");
    MeshManager::DumbModels();
}
void InitModels()
{
    Logger::Stage("loading", "Main", "INITIALIZING MODELS");
    backpack = ModelManager::GetModel("backpack");
    train = ModelManager::GetModel("train");
    cube = ModelManager::GetModel("cube");
    cubes = ModelManager::GetModel("cubes");
}
void SetObjectProperties()
{
    Logger::Stage("loading", "Main", "SETTING OBJECTS PROPERTIES");
    FPSCamera &camera = scene["player"].GetComponent<FPSCamera>();

    DirectionalLight &sun = scene["sun"].GetComponent<DirectionalLight>();
    DirectionalLight &moon = scene["moon"].GetComponent<DirectionalLight>();
    PointLight &lamp_white = scene["lamp_white"].GetComponent<PointLight>();
    PointLight &lamp_magenta = scene["lamp_magenta"].GetComponent<PointLight>();
    SpotLight &flashlight = scene["flashlight"].GetComponent<SpotLight>();
    SpotLight &lamp = scene["lamp"].GetComponent<SpotLight>();

    camera.SetPos(glm::vec3(0.0f, 0.0f, 100.0f));

    moon.ambient = glm::vec3(0.2f, 0.56f, 1.0f) * 0.02f;
    moon.diffuse = glm::vec3(0.2f, 0.56f, 1.0f) * 0.3f;
    moon.specular = glm::vec3(0.2f, 0.56f, 1.0f) * 0.6f;

    sun.ambient = glm::vec3(1.0f, 0.73f, 0.2f) * 0.02f;
    sun.diffuse = glm::vec3(1.0f, 0.73f, 0.2f) * 0.6f;
    sun.specular = glm::vec3(1.0f, 0.73f, 0.2f) * 0.3f;

    lamp_white.ambient = COLOR_WHITE * 0.1f;
    lamp_white.diffuse = COLOR_WHITE * 0.3f;
    lamp_white.specular = COLOR_WHITE * 0.6f;
    lamp_white.clq = glm::vec3(1.0f, 0.007f, 0.0002f);

    lamp_magenta.ambient = COLOR_MAGENTA * 0.1f;
    lamp_magenta.diffuse = COLOR_MAGENTA * 0.6f;
    lamp_magenta.specular = COLOR_MAGENTA * 0.3f;
    lamp_magenta.clq = glm::vec3(1.0f, 0.022f, 0.0019f);

    flashlight.ambient = COLOR_WHITE * 0.1f;
    flashlight.diffuse = COLOR_WHITE * 0.4f;
    flashlight.specular = COLOR_WHITE * 0.4f;
    flashlight.clq = glm::vec3(1.0f, 0.007f, 0.0002f);
    flashlight.inner_cone_cos = cos(glm::radians(12.5));
    flashlight.outer_cone_cos = cos(glm::radians(20.5));

    lamp.ambient = COLOR_RED * 0.1f;
    lamp.diffuse = COLOR_RED * 0.6f;
    lamp.specular = COLOR_RED * 0.4f;
    lamp.clq = glm::vec3(1.0f, 0.022f, 0.0019f);
    lamp.position = glm::vec3(0.0f, 50.0f, 0.0f);
    lamp.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lamp.inner_cone_cos = cos(glm::radians(12.5));
    lamp.outer_cone_cos = cos(glm::radians(17.5));
}

void ProcessInput()
{
    FPSCamera &camera = scene["player"].GetComponent<FPSCamera>();
    float sensitivity = 0.07;
    float speed = 0.5;

    input_manager.ReadFrame();

    if (input_manager.KeyReleased(GLFW_KEY_TAB))
    {
        if (cursor_mode)
        {
            glfwSetInputMode(application_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_mode = false;
        }
        else
        {
            glfwSetInputMode(application_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursor_mode = true;
        }
    }

    if (input_manager.KeyReleased(GLFW_KEY_F11))
    {
        if (application_window.GetScreenSizeState() == ScreenSizeState::fullscreen)
        {
            application_window.SetWindowed();
        }
        else
        {
            int monitor_id = 0;
            if (input_manager.KeyDown(GLFW_KEY_1))
            {
                monitor_id = 1;
            }
            application_window.SetFullScreen(monitor_id);
        }
        input_manager.SetWindow(application_window.GetWindow());
    }

    int width, height;
    glfwGetWindowSize(application_window.GetWindow(), &width, &height);
    camera.SetAspect(double(width) / height);

    if (!cursor_mode)
    {
        return;
    }

    camera.RotateY(input_manager.CursorXOffset() * sensitivity);
    camera.RotateX(input_manager.CursorYOffset() * sensitivity / 16 * 9);

    if (input_manager.KeyDown(GLFW_KEY_LEFT_SHIFT))
    {
        speed = 50.0f;
    }
    if (input_manager.KeyDown(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(application_window.GetWindow(), true);
    }
    if (input_manager.KeyDown(GLFW_KEY_S))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, -1.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_W))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, 1.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_D))
    {
        camera.MoveInLocal(glm::vec3(1.0f, 0.0f, 0.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_A))
    {
        camera.MoveInLocal(glm::vec3(-1.0f, 0.0f, 0.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_SPACE))
    {
        camera.MoveInWorld(glm::vec3(0.0f, 1.0f, 0.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_C))
    {
        camera.MoveInWorld(glm::vec3(0.0f, -1.0f, 0.0f) * speed * input_manager.FrameTime());
    }
}
void UpdateWorld()
{
    static size_t frame;
    // Logger::Stage("rendering", "Frame: " + std::to_string(frame++));
    glm::vec3 center(0, 20, 0);

    sun.direction = glm::vec3(sin(glfwGetTime() / 5), cos(glfwGetTime() / 5), 0);
    moon.direction = glm::vec3(sin(glfwGetTime() / 5 + glm::pi<float>()), cos(glfwGetTime() / 5 + glm::pi<float>()), 0);

    lamp_white.position = center + glm::vec3(sin(glfwGetTime() / 3), 0, cos(glfwGetTime() / 3)) * 100.0f;
    lamp_magenta.position = center + glm::vec3(sin(glfwGetTime() / 5) * 200, 0, 30);

    flashlight.position = camera.GetPos();
    flashlight.direction = camera.GetDir();
}
void Draw()
{
    sp_textured_phong.SetMat4fv("projection", camera.GetProjectionMatrix());
    sp_textured_phong.SetMat4fv("view", camera.GetViewMatrix());
    sp_textured_phong.SetVec3f("view_pos", camera.GetPos());
    sp_colored_phong.SetMat4fv("projection", camera.GetProjectionMatrix());
    sp_colored_phong.SetMat4fv("view", camera.GetViewMatrix());
    sp_colored_phong.SetVec3f("view_pos", camera.GetPos());

    light_manager.UpdateLights(sp_textured_phong);
    light_manager.UpdateLights(sp_colored_phong);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    e = glm::mat4(1.0);
    sp_textured_phong.SetMat4fv("rotation", e);
    e = glm::translate(e, glm::vec3(0, 50, 0));
    sp_textured_phong.SetMat4fv("model", e);
    backpack->Draw(sp_textured_phong);

    e = glm::mat4(1.0);
    sp_colored_phong.SetMat4fv("rotation", e);
    sp_colored_phong.SetMat4fv("model", e);
    sp_colored_phong.SetVec3f("color", 1.0, 0.5, 1.0);

    e = glm::mat4(1.0);

    for (size_t i = 0; i < 100; ++i)
    {
        e = glm::translate(e, glm::vec3(0, 0, 10));
        sp_colored_phong.SetMat4fv("model", e);
        train->Draw(sp_colored_phong);
    }
}