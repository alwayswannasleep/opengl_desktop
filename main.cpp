#include <iostream>
#include "GLFW/glfw3.h"
#include <thread>

long long getCurrentTime() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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
    glViewport(0, 0, 800, 600);

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, [](auto window, auto width, auto height) {
        glViewport(0, 0, width, height);
    });

    glfwSetWindowIconifyCallback(window, [](auto, auto iconified) {
        if (iconified) {
            glfwWaitEvents();
        }
    });

    int fps = 0;
    auto start = getCurrentTime();
    while (!glfwWindowShouldClose(window)) {
        auto frameStart = getCurrentTime();

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