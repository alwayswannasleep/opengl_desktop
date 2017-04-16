#ifdef APPLE
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <iostream>
#include "GLFW/glfw3.h"
#include <thread>
#include <ext.hpp>
#include "Program.h"
#include "Camera.h"

#define DEFAULT_FPS_TARGET 60
#define ICONIFIED_FPS_TARGET 5

Camera camera;

void renderFrame(Program i, GLuint i1);

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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    glfwSetFramebufferSizeCallback(window, onWindowSizeChanged);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_FRONT);

    glDepthFunc(GL_LEQUAL);
    glDepthRange(1, 100);

    GLfloat vertices[] = {
            0.5f, 0.5f, 0.0f,  // Top Right
            0.5f, -0.5f, 0.0f,  // Bottom Right
            -0.5f, -0.5f, 0.0f,  // Bottom Left
            -0.5f, 0.5f, 0.0f   // Top Left
    };
    GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3,   // First Triangle
            1, 2, 3    // Second Triangle
    };

    Program program("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");

    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);

    GLuint vboId;
    glGenBuffers(1, &vboId);

    GLuint eboId;
    glGenBuffers(1, &eboId);

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    camera.setPosition({-5, 0, 4});
    camera.setTarget({0, 0, -1});

    int fps = 0;
    auto start = getCurrentTime();
    while (!glfwWindowShouldClose(window)) {
        auto frameStart = getCurrentTime();

        renderFrame(program, vaoId);

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
        long long int targetTime = (long long int) (1000.f /
                                                    (glfwGetWindowAttrib(window, GLFW_ICONIFIED) ? ICONIFIED_FPS_TARGET
                                                                                                 : DEFAULT_FPS_TARGET));
        if (delta <= targetTime) {
            std::this_thread::sleep_for(std::chrono::nanoseconds((targetTime - delta) * 1000));
        }
    }

    glfwTerminate();
    return 0;
}

void renderFrame(Program program, GLuint vao) {
    glClearColor(0.4f, 0.4f, 0.4f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.use();


    glm::mat4 translate;
    glm::translate(translate, glm::vec3(0, 0, -2));

    glBindVertexArray(vao);
    glm::mat4 perspective = glm::perspective<float>(45, 800 / 600, 1, 100);
    glUniformMatrix4fv(program.getUniformLocation("viewMatrix"), 1, GL_FALSE,
                       glm::value_ptr(perspective * camera.getViewMatrix()));
    glUniformMatrix4fv(program.getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(translate));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}