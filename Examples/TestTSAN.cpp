#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <thread>
#include <vector>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    GLFWwindow* window;
    const char* title;
    float       r, g, b;
} Thread;

static std::atomic<int> running = GLFW_TRUE;

static void error_callback(int, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static int thread_main(void* data) {
    const Thread* thread = reinterpret_cast<Thread*>(data);

    glfwMakeContextCurrent(thread->window);
    glfwSwapInterval(1);

    while (running) {
        const float v = (float)fabs(sin(glfwGetTime() * 2.f));
        glClearColor(thread->r * v, thread->g * v, thread->b * v, 0.f);

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(thread->window);
    }

    glfwMakeContextCurrent(NULL);
    return 0;
}

int main(void) {
    int    i;
    Thread threads[] = {
        {NULL,   "Red", 1.f, 0.f, 0.f},
    };
    const int count = sizeof(threads) / sizeof(Thread);

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) exit(EXIT_FAILURE);

    for (i = 0; i < count; i++) {
        glfwWindowHint(GLFW_POSITION_X, 200 + 250 * i);
        glfwWindowHint(GLFW_POSITION_Y, 200);

        threads[i].window = glfwCreateWindow(200, 200, threads[i].title, NULL, NULL);
        if (!threads[i].window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetKeyCallback(threads[i].window, key_callback);
    }

    glfwMakeContextCurrent(threads[0].window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwMakeContextCurrent(NULL);

    std::vector<std::thread> workers;

    for (i = 0; i < count; i++) {
        workers.emplace_back([data = &threads[i]]() { thread_main(data); });
    }

    while (running) {
        glfwWaitEvents();

        for (i = 0; i < count; i++) {
            if (glfwWindowShouldClose(threads[i].window)) running = GLFW_FALSE;
        }
    }

    for (i = 0; i < count; i++) glfwHideWindow(threads[i].window);

    for (i = 0; i < count; i++) {
        workers[i].join();
    }

    exit(EXIT_SUCCESS);
}
