#include "../Engine/Addition/de_lib.h"

using namespace DE;

bool cursor_mode = true;

Window application_window;

std::unordered_map<std::string, Entity> scene;

struct Drawable {};

struct LinearManipulator {
    glm::vec3 dir;
    double radius;
    double current_time;
    double speed;

    LinearManipulator()
        : dir(0, 0, 1),
          radius(10),
          current_time(0),
          speed(1){

          };
    glm::vec3 Update(double dt) {
        current_time += dt;
        return dir * float((std::sin(current_time * speed) - std::sin((current_time - dt) * speed)) * radius);
    }
};
struct ScaleManipulator {
    double min_scale, max_scale;
    double time_offset;

    ScaleManipulator() : min_scale(0), max_scale(2), time_offset(0) {
    }
    glm::vec3 GetScale() {
        return glm::vec3(((max_scale + min_scale) / 2) + ((max_scale - min_scale) / 2) * (std::sin(time_offset + glfwGetTime())));
    }
};

class MovingSystem : public System {
public:
    MovingSystem() {
    }

    void Update(double dt) override {
        DE_FTR_ENTER("Moving System");
        scene["flashlight"].GetComponent<SpotLight>().position = scene["player"].GetComponent<FPSCamera>().GetPos();
        scene["flashlight"].GetComponent<SpotLight>().direction = scene["player"].GetComponent<FPSCamera>().GetDir();

        auto& manipulators = GetComponentArray<LinearManipulator>();
        auto& positions = GetComponentArray<Transformation>();
        auto& scales = GetComponentArray<ScaleManipulator>();
        auto& point_lights = GetComponentArray<PointLight>();
        for (auto [entity_id, linear_manipulator] : manipulators) {
            positions[entity_id].MoveInWorld(linear_manipulator.Update(dt));
        }
        for (auto [entity_id, scale] : scales) {
            positions[entity_id].SetScale(scale.GetScale());
        }
        for (auto [entity_id, point_light] : point_lights) {
            point_light.position = positions[entity_id].GetPos();
        }
        DE_FTR_LEAVE();
    }
};
class DrawSystem : public System {
public:
    DrawSystem() {
    }

    void Update(double dt) override {
        DE_FTR_ENTER("Draw System");

        auto& drawables = GetComponentArray<Drawable>();
        auto& models = GetComponentArray<RenderModel>();
        auto& shaders = GetComponentArray<GLShaderProgram>();
        auto& transformations = GetComponentArray<Transformation>();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto& camera = scene["player"].GetComponent<FPSCamera>();
        for (auto [entity_id, shader] : shaders) {
            shader.SetMat4fv("projection", camera.GetProjectionMatrix());
            shader.SetMat4fv("view", camera.GetViewMatrix());
            shader.SetVec3f("view_pos", camera.GetPos());
        }
        for (auto [entity_id, drawable] : drawables) {
            transformations[entity_id].Update();
            GLShaderProgram& shader_program = shaders[entity_id];
            shader_program.SetMat4fv("rotation", transformations[entity_id].GetRotationMatrix());
            shader_program.SetMat4fv("model", transformations[entity_id].GetModelMatrix());
            RenderModel& model = models[entity_id];
            for (const auto& mesh : model.render_meshes) {
                shader_program.SetMaterial("material", mesh.material);
                mesh.vertex_array.Bind();
                glDrawElements(GL_TRIANGLES, mesh.indices_amount, GL_UNSIGNED_INT, 0);
            }
            glBindVertexArray(0);
        }
        DE_FTR_LEAVE();
    }
};

void Initialize();

void RegisterSystems();
void CreateEntities();
void LoadShaders();
void LoadModels();
void SetObjectProperties();

void ProcessInput();
void UpdateWorld();
void Draw();

int main() {
    Initialize();
    RegisterSystems();
    CreateEntities();
    LoadShaders();
    LoadModels();
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

void Initialize() {
    std::cout << WORKING_DIR << std::endl;
    Logger::Open(LOG_DIR / "loading.txt", "loading");
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    Logger::Stage("loading", "Main", "INITIALIZETION");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    deInitialize();
    application_window.Init("First");
    application_window.SetProcessInputFunc(ProcessInput);
    Input::SetWindow(application_window.GetWindow());
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
}

void RegisterSystems() {
    SystemManager::RegisterSystem<MovingSystem>();
    SystemManager::RegisterSystem<LightManager>();
    SystemManager::RegisterSystem<DrawSystem>();

    SystemManager::AddOrder<MovingSystem, LightManager>();
    SystemManager::AddOrder<LightManager, DrawSystem>();
    SystemManager::CalculateOrder();
}
void CreateEntities() {
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
    scene["sponza"].AddComponent<GLShaderProgram>();

    scene["train"].AddComponent<Transformation>();
    scene["lamp_white"].AddComponent<Transformation>();
    scene["lamp_magenta"].AddComponent<Transformation>();
    scene["lamp"].AddComponent<Transformation>();
    scene["flashlight"].AddComponent<Transformation>();
    scene["surface"].AddComponent<Transformation>();

    scene["backpack"].AddComponent<Drawable>();
    scene["train"].AddComponent<Drawable>();
    scene["surface"].AddComponent<Drawable>();

    scene["train"].AddComponent<GLShaderProgram>();
    scene["backpack"].AddComponent<GLShaderProgram>();

    scene["backpack"].AddComponent<LinearManipulator>();

    scene["sun"].AddComponent<DirectionalLight>();
    scene["moon"].AddComponent<DirectionalLight>();
    scene["lamp_white"].AddComponent<PointLight>();
    scene["lamp_magenta"].AddComponent<PointLight>();
    scene["flashlight"].AddComponent<SpotLight>();
    scene["lamp"].AddComponent<SpotLight>();
}
void LoadShaders() {
    Logger::Stage("loading", "Main", "LOADING SHADERS");
    scene["colored_phong"].GetComponent<UniqueShader>().shader_program.SmartInit(SHADER_DIR / "ColoredPhong");
    scene["textured_phong"].GetComponent<UniqueShader>().shader_program.SmartInit(SHADER_DIR / "TexturedPhong");
    scene["train"].AddComponent<GLShaderProgram>(scene["colored_phong"].GetComponent<UniqueShader>().shader_program);
    scene["surface"].AddComponent<GLShaderProgram>(scene["colored_phong"].GetComponent<UniqueShader>().shader_program);
    scene["backpack"].AddComponent<GLShaderProgram>(scene["textured_phong"].GetComponent<UniqueShader>().shader_program);
    scene["sponza"].AddComponent<GLShaderProgram>(scene["textured_phong"].GetComponent<UniqueShader>().shader_program);

    Logger::Info("loading", "Main", "Shaders loaded.");
}

void LoadModels() {
    Logger::Stage("loading", "Main", "LOADING MODELS");
    RenderModelData train, backpack, cube, cubes, sponza;
    RenderModel r_train, r_backpack, r_cube, r_cubes, r_sponza;

    ModelLoader::LoadModel(MODEL_DIR / "Train" / "train.obj", train);
    ModelLoader::LoadModel(MODEL_DIR / "Backpack" / "backpack.obj", backpack);
    ModelLoader::LoadModel(MODEL_DIR / "Cube" / "cube.obj", cube);
    ModelLoader::LoadModel(MODEL_DIR / "Cubes" / "cubes.obj", cubes);
    ModelLoader::LoadModel(MODEL_DIR / "Sponza" / "sponza.obj", sponza);

    train.Compress();
    backpack.Compress();
    cubes.Compress();
    cube.Compress();

    r_sponza.FillData(sponza);
    r_train.FillData(train);
    r_backpack.FillData(backpack);
    r_cube.FillData(cube);
    r_cubes.FillData(cubes);

    scene["backpack"].AddComponent<RenderModel>(r_backpack);
    scene["train"].AddComponent<RenderModel>(r_train);
    scene["surface"].AddComponent<RenderModel>(r_cubes);
    scene["cubes"].AddComponent<RenderModel>(r_cubes);
    scene["sponza"].AddComponent<RenderModel>(r_sponza);

    scene["lamp_white"].AddComponent<RenderModel>(r_cube);
    scene["lamp_magenta"].AddComponent<RenderModel>(r_cube);
    scene["lamp_white"].AddComponent<Drawable>();
    scene["lamp_magenta"].AddComponent<Drawable>();
    scene["lamp_white"].AddComponent<GLShaderProgram>(scene["colored_phong"].GetComponent<UniqueShader>().shader_program);
    scene["lamp_magenta"].AddComponent<GLShaderProgram>(scene["colored_phong"].GetComponent<UniqueShader>().shader_program);

    for (int i = 0; i < 200; ++i) {
        scene["train" + std::to_string(i)].AddComponent<RenderModel>(scene["train"].GetComponent<RenderModel>());
        scene["train" + std::to_string(i)].GetComponent<Transformation>().SetPos(glm::vec3(0, 100, 300 - i * 12));
        scene["train" + std::to_string(i)].AddComponent<GLShaderProgram>(scene["train"].GetComponent<GLShaderProgram>());
        scene["train" + std::to_string(i)].AddComponent<LinearManipulator>();
        scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().current_time = i * 3.14 / 16;
        scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().radius = 15;
        scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().dir = glm::vec3(std::sin(i * 3.14 / 16), std::cos(i * 3.14 / 16), 0);
        scene["train" + std::to_string(i)].GetComponent<LinearManipulator>().speed = std::sin(i * 3.14 / 8) + 1;
        scene["train" + std::to_string(i)].AddComponent<ScaleManipulator>();
        scene["train" + std::to_string(i)].GetComponent<ScaleManipulator>().min_scale = -1;
        scene["train" + std::to_string(i)].GetComponent<ScaleManipulator>().max_scale = 1;
        scene["train" + std::to_string(i)].GetComponent<ScaleManipulator>().time_offset = i * 3.14 / 16;
        scene["train" + std::to_string(i)].AddComponent<Drawable>();
    }
}
void SetObjectProperties() {
    Logger::Stage("loading", "Main", "SETTING OBJECTS PROPERTIES");

    scene["sponza"].GetComponent<Transformation>().SetPos(glm::vec3(0, 50, 0));
    scene["sponza"].GetComponent<Transformation>().SetScale(glm::vec3(0.01));

    scene["backpack"].GetComponent<Transformation>().SetPos(glm::vec3(30, 0, 0));
    scene["backpack"].GetComponent<LinearManipulator>().radius = 0;

    scene["surface"].GetComponent<Transformation>().SetPos(glm::vec3(0, -10, 0));
    scene["surface"].GetComponent<Transformation>().SetScale(glm::vec3(500, 0.1, 500));

    FPSCamera& camera = scene["player"].GetComponent<FPSCamera>();
    DirectionalLight& sun = scene["sun"].GetComponent<DirectionalLight>();
    DirectionalLight& moon = scene["moon"].GetComponent<DirectionalLight>();
    PointLight& lamp_white = scene["lamp_white"].GetComponent<PointLight>();
    PointLight& lamp_magenta = scene["lamp_magenta"].GetComponent<PointLight>();
    SpotLight& flashlight = scene["flashlight"].GetComponent<SpotLight>();
    SpotLight& lamp = scene["lamp"].GetComponent<SpotLight>();

    scene["lamp_white"].GetComponent<LinearManipulator>().dir = glm::vec3(1, 0, 0);
    scene["lamp_white"].GetComponent<LinearManipulator>().radius = 50;
    scene["lamp_white"].GetComponent<LinearManipulator>().speed = 0.5;
    scene["lamp_white"].GetComponent<Transformation>().SetPos(glm::vec3(0, 20, 0));

    scene["lamp_magenta"].GetComponent<LinearManipulator>().dir = glm::vec3(0, 0, 1);
    scene["lamp_magenta"].GetComponent<LinearManipulator>().radius = 50;
    scene["lamp_magenta"].GetComponent<LinearManipulator>().speed = 0.5;
    scene["lamp_magenta"].GetComponent<Transformation>().SetPos(glm::vec3(0, 10, 0));

    camera.SetPos(glm::vec3(0.0f, 0.0f, 10.0f));

    moon.ambient = glm::vec3(0.2f, 0.56f, 1.0f) * 0.02f;
    moon.diffuse = glm::vec3(0.2f, 0.56f, 1.0f) * 0.3f;
    moon.specular = glm::vec3(0.2f, 0.56f, 1.0f) * 0.6f;
    moon.direction = glm::normalize(glm::vec3(0, -1, 2));

    sun.ambient = glm::vec3(1.0f, 0.73f, 0.2f) * 0.02f;
    sun.diffuse = glm::vec3(1.0f, 0.73f, 0.2f) * 0.6f;
    sun.specular = glm::vec3(1.0f, 0.73f, 0.2f) * 0.3f;
    sun.direction = glm::normalize(glm::vec3(0, 1, -2));

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

void ProcessInput() {
    FPSCamera& camera = scene["player"].GetComponent<FPSCamera>();
    float sensitivity = 0.07;
    float speed = 15;

    Input::ReadFrame();

    if (Input::KeyReleased(GLFW_KEY_TAB)) {
        if (cursor_mode) {
            glfwSetInputMode(application_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_mode = false;
        } else {
            glfwSetInputMode(application_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursor_mode = true;
        }
    }

    if (Input::KeyReleased(GLFW_KEY_F11)) {
        if (application_window.GetScreenSizeState() == ScreenSizeState::fullscreen) {
            application_window.SetWindowed();
        } else {
            int monitor_id = 0;
            if (Input::KeyDown(GLFW_KEY_1)) {
                monitor_id = 1;
            }
            application_window.SetFullScreen(monitor_id);
        }
        Input::SetWindow(application_window.GetWindow());
    }

    int width, height;
    glfwGetWindowSize(application_window.GetWindow(), &width, &height);
    if (height != 0 && width != 0) {
        camera.SetAspect(double(width) / height);
    }
    if (!cursor_mode) {
        return;
    }

    camera.RotateY(Input::CursorXOffset() * sensitivity);
    camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

    if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT)) {
        speed = 50.0f;
    }
    if (Input::KeyDown(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(application_window.GetWindow(), true);
    }
    if (Input::KeyDown(GLFW_KEY_S)) {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, -1.0f) * speed * Input::FrameTime());
    }
    if (Input::KeyDown(GLFW_KEY_W)) {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, 1.0f) * speed * Input::FrameTime());
    }
    if (Input::KeyDown(GLFW_KEY_D)) {
        camera.MoveInLocal(glm::vec3(1.0f, 0.0f, 0.0f) * speed * Input::FrameTime());
    }
    if (Input::KeyDown(GLFW_KEY_A)) {
        camera.MoveInLocal(glm::vec3(-1.0f, 0.0f, 0.0f) * speed * Input::FrameTime());
    }
    if (Input::KeyDown(GLFW_KEY_SPACE)) {
        camera.MoveInWorld(glm::vec3(0.0f, 1.0f, 0.0f) * speed * Input::FrameTime());
    }
    if (Input::KeyDown(GLFW_KEY_C)) {
        camera.MoveInWorld(glm::vec3(0.0f, -1.0f, 0.0f) * speed * Input::FrameTime());
    }
}

