#include "../Engine/Addition/de_lib.h"

ApplicationWindow application_window;
ShaderProgram sp_basic_texture;
VAO rect;

void Initialize();
void InitShaders();

void ProcessInput();
void UpdateWorld();
void Draw();

int main() {
    Initialize();
    InitShaders();

    std::vector<float> vertices = {
        0.5f,  0.5f,  0.0f,  // top right
        0.5f,  -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f   // top left
    };
    std::vector<unsigned int> indices = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    rect.Init(vertices, indices);
    rect.SetFloatAttribute(0, 3, GL_FALSE, 3, 0);

    application_window.StartLoop();

    return 0;
}

void Initialize() {
    std::cout << WORKING_DIR << std::endl;
    Logger::Open(LOG_DIR / "loading.txt", "loading");
    Logger::Info("Logger initialized.", "Main", "loading");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    application_window.Init("First");
    application_window.SetProcessInputFunc(ProcessInput);
    application_window.SetUpdateWorldFunc(UpdateWorld);
    application_window.SetDrawFunc(Draw);
    application_window.MakeCurrentContext();
}
void InitShaders() {
    sp_basic_texture.SmartInit(SHADER_DIR / "BasicTriangle");
}

void ProcessInput() {
    glfwPollEvents();
    if (glfwGetKey(application_window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(application_window.GetWindow(), true);
    }
}
void UpdateWorld() {
}
void Draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    sp_basic_texture.Use();
    rect.Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}