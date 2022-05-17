#include "../Engine/Addition/de_lib.h"

int main(void) {
    GLFWwindow* window1;
    GLFWwindow* window2;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window1 = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window1) {
        glfwTerminate();
        return -1;
    }

    /* Create a second windowed mode window and its OpenGL context */
    window2 = glfwCreateWindow(640, 480, "World Hello", NULL, NULL);
    if (!window2) {
        glfwTerminate();
        return -1;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2)) {
        /* Make the window's context current */
        glfwMakeContextCurrent(window1);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "F";
        }

        /* Render here */
        glClearColor(sin(glfwGetTime() + 1) / 2, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers */
        glfwSwapBuffers(window1);

        /* Make the second window's context current */
        glfwMakeContextCurrent(window2);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "F";
        }
        /* Render here */
        glClearColor(0.2, 0.3f, sin(glfwGetTime() + 1) / 2, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers */
        glfwSwapBuffers(window2);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}