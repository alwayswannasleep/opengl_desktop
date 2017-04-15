#ifdef APPLE
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>
#include "GLFW/glfw3.h"
#include <thread>

void renderFrame();

long long getCurrentTime() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void onWindowSizeChanged(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(800, 600, "OpenGL test", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Can not create glfw window!\n";
        glfwTerminate();
        return -1;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwMakeContextCurrent(window);

#ifndef APPLE
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing glew. Exiting.\n";
        return -1;
    }
#endif

    glViewport(0, 0, width, height);
    glfwSetWindowSizeCallback(window, onWindowSizeChanged);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glDepthFunc(GL_LEQUAL);
    glDepthRange(1, 100);

    int fps = 0;
    auto start = getCurrentTime();
    while (!glfwWindowShouldClose(window)) {
        auto frameStart = getCurrentTime();

        renderFrame();

        glfwPollEvents();
        glfwSwapBuffers(window);

        fps++;

        auto frameEnd = getCurrentTime();
        if (frameEnd - start >= 1000) {
            std::cout << "Fps counter: " << fps << std::endl;
            fps = 0;
            start = frameEnd;
        }

        auto delta = frameEnd - frameStart;
        if (delta <= 1000.f / 60) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(delta * 1000));
        }
    }

    glfwTerminate();
    return 0;
}

void renderFrame() {
    glClearColor(0.4f, 0.4f, 0.4f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}