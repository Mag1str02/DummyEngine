#include "../Objects/Models/Model/model.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "../Memory/VAO/vao.h"
#include "../Objects/Cameras/FPSCamera/fps_camera.h"
#include "../Objects/LightSources/DirectionalLight/directional_light.h"
#include "../Objects/LightSources/LightManager/light_manager.h"
#include "../Objects/LightSources/PointLight/point_light.h"
#include "../Objects/LightSources/SpotLight/spot_light.h"
#include "../UnSorted/Config/config.h"
#include "../UnSorted/InputManager/input_manager.h"
#include "../UnSorted/Logger/logger.h"
#include "../Wrappings/ApplicationWindow/application_window.h"
#include "../Wrappings/ShaderProgram/shader_program.h"

glm::mat4 e = glm::mat4(1.0f);

bool cursor_mode = true;

InputManager input_manager;
ApplicationWindow application_window;
ShaderProgram sp_textured_phong, sp_colored_phong;
FPSCamera camera;
Model backpack, train, cube;

LightManager light_manager;
DirectionalLight sun, moon;
PointLight lamp_white, lamp_magenta;
SpotLight flashlight, lamp;

void Initialize();
void LoadShaders();
void LoadModels();
void SetObjectProperties();

void ProcessInput();
void UpdateWorld();
void Draw();

int main() {
    Initialize();
    LoadShaders();
    LoadModels();
    SetObjectProperties();

    application_window.StartLoop();

    return 0;
}

void Initialize() {
    std::cout << WORKING_DIR << std::endl;
    Logger::Open(LOG_DIR / "loading.txt", "loading");
    Logger::Info("loading", "Logger initialized.");
    application_window.Init("First");
    application_window.SetProcessInputFunc(ProcessInput);
    application_window.SetUpdateWorldFunc(UpdateWorld);
    application_window.SetDrawFunc(Draw);
    input_manager.SetWindow(application_window.GetWindow());
}
void LoadShaders() {
    Logger::Info("loading", "Loading shaders.");
    sp_textured_phong.SmartInit(SHADER_DIR / "TexturedPhong");
    sp_colored_phong.SmartInit(SHADER_DIR / "ColoredPhong");

    light_manager.AddDirectionalLight(&sun);
    light_manager.AddDirectionalLight(&moon);

    light_manager.AddPointLight(&lamp_white);
    light_manager.AddPointLight(&lamp_magenta);

    light_manager.AddSpotLight(&flashlight);
    light_manager.AddSpotLight(&lamp);

    Logger::Info("loading", "Shaders loaded.");
}
void LoadModels() {
    backpack.Init(MODEL_DIR / "Backpack" / "backpack.obj");
    train.Init(MODEL_DIR / "Train" / "train.obj");
    cube.Init(MODEL_DIR / "Cube" / "cube.obj");
}
void SetObjectProperties() {
    camera.SetPos(glm::vec3(0.0f, 0.0f, 10.0f));

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

void ProcessInput() {
    float sensitivity = 0.07;
    float speed = 0.5;

    input_manager.ReadFrame();

    if (input_manager.KeyReleased(GLFW_KEY_TAB)) {
        if (cursor_mode) {
            glfwSetInputMode(application_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_mode = false;
        } else {
            glfwSetInputMode(application_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursor_mode = true;
        }
    }

    if (input_manager.KeyReleased(GLFW_KEY_F11)) {
        if (application_window.GetScreenSizeState() == ScreenSizeState::fullscreen) {
            application_window.SetWindowed();
        } else {
            int monitor_id = 0;
            if (input_manager.KeyDown(GLFW_KEY_1)) {
                monitor_id = 1;
            }
            application_window.SetFullScreen(monitor_id);
        }
        input_manager.SetWindow(application_window.GetWindow());
    }

    int width, height;
    glfwGetWindowSize(application_window.GetWindow(), &width, &height);
    camera.SetAspect(double(width) / height);

    if (!cursor_mode) {
        return;
    }

    camera.RotateY(input_manager.CursorXOffset() * sensitivity);
    camera.RotateX(input_manager.CursorYOffset() * sensitivity / 16 * 9);

    if (input_manager.KeyDown(GLFW_KEY_LEFT_SHIFT)) {
        speed = 50.0f;
    }
    if (input_manager.KeyDown(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(application_window.GetWindow(), true);
    }
    if (input_manager.KeyDown(GLFW_KEY_S)) {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, -1.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_W)) {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, 1.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_D)) {
        camera.MoveInLocal(glm::vec3(1.0f, 0.0f, 0.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_A)) {
        camera.MoveInLocal(glm::vec3(-1.0f, 0.0f, 0.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_SPACE)) {
        camera.MoveInWorld(glm::vec3(0.0f, 1.0f, 0.0f) * speed * input_manager.FrameTime());
    }
    if (input_manager.KeyDown(GLFW_KEY_C)) {
        camera.MoveInWorld(glm::vec3(0.0f, -1.0f, 0.0f) * speed * input_manager.FrameTime());
    }
}
void UpdateWorld() {
    glm::vec3 center(0, 20, 0);

    sun.direction = glm::vec3(sin(glfwGetTime() / 5), cos(glfwGetTime() / 5), 0);
    moon.direction = glm::vec3(sin(glfwGetTime() / 5 + glm::pi<float>()), cos(glfwGetTime() / 5 + glm::pi<float>()), 0);

    lamp_white.position = center + glm::vec3(sin(glfwGetTime() / 3), 0, cos(glfwGetTime() / 3)) * 100.0f;
    lamp_magenta.position = center + glm::vec3(sin(glfwGetTime() / 5) * 200, 0, 30);

    flashlight.position = camera.GetPos();
    flashlight.direction = camera.GetDir();

    sp_textured_phong.SetMat4fv("rotation", e);
    sp_textured_phong.SetMat4fv("model", e);
    sp_colored_phong.SetVec3f("view_pos", camera.GetPos());
    sp_colored_phong.SetMat4fv("rotation", e);
    sp_colored_phong.SetMat4fv("model", e);
    sp_colored_phong.SetVec3f("view_pos", camera.GetPos());
}
void Draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    light_manager.UpdateLights(sp_textured_phong);
    light_manager.UpdateLights(sp_colored_phong);
    e = glm::mat4(1.0);
    sp_colored_phong.SetMat4fv("rotation", e);
    sp_colored_phong.SetMat4fv("model", e);
    sp_colored_phong.SetVec3f("color", 1.0, 0.5, 1.0);
    sp_textured_phong.SetMat4fv("rotation", e);
    e = glm::translate(e, glm::vec3(20, 0, 0));
    sp_textured_phong.SetMat4fv("model", e);
    backpack.Draw(camera, sp_textured_phong);

    e = glm::mat4(1.0);
    train.Draw(camera, sp_colored_phong);
    e = glm::scale(e, glm::vec3(500, 1, 500));
    e = glm::translate(e, glm::vec3(0, -3, 0));
    sp_colored_phong.SetVec3f("color", 0.5, 1.0, 1.0);
    sp_colored_phong.SetMat4fv("model", e);
    cube.Draw(camera, sp_colored_phong);

    // glDisable(GL_DEPTH_TEST);
    // sp_basic_texture.Use();
    // rect.Use();
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}