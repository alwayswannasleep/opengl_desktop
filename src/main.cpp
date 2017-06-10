#include "glwrapper.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include <thread>
#include <glm/ext.hpp>
#include "Camera.h"
#include "actors/Actor.h"
#include "actors/Cube.h"
#include "logs.h"
#include "assimp/Importer.hpp"
#include "Texture.h"

#define DEFAULT_FPS_TARGET 60
#define ICONIFIED_FPS_TARGET 5
#define CAMERA_SPEED 5;

Camera camera;
glm::mat4 perspective;
bool pressedKeys[1024];

void renderFrame(Actor *pActor);

void updateMovements(long long int delta);

long long getCurrentTime() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void onWindowSizeChanged(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void onKeyPressed(GLFWwindow *window, int key, int scancode, int action, int mode) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            if (action == GLFW_PRESS) {
                pressedKeys[key] = true;
            } else if (action == GLFW_RELEASE) {
                pressedKeys[key] = false;
            }
            break;
    }
}

void onCursorPositionChanged(GLFWwindow *window, double xPos, double yPos) {
    static double previousX = 400;
    static double previousY = 300;

    float xOffset = static_cast<float>(xPos - previousX);
    float yOffset = static_cast<float>(previousY - yPos);
    previousX = xPos;
    previousY = yPos;

    GLfloat sensitivity = 0.05f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera.handleRotations(xOffset, yOffset);
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
        LOGM("Can not create glfw window!");
        std::cerr << "\n";
        glfwTerminate();
        return -1;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        LOGM("Error initializing glew. Exiting.\n");
        return -1;
    }
#endif

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, onWindowSizeChanged);

    glfwSetKeyCallback(window, onKeyPressed);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, onCursorPositionChanged);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    glDepthFunc(GL_LEQUAL);
    glDepthRange(1, 100);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    camera.setPosition(glm::vec3(0, 0, 4));

    Actor *actor = new Cube();

    perspective = glm::perspective<float>(45, 800 / 600, 1, 100);

    int fps = 0;
    auto start = getCurrentTime();
    while (!glfwWindowShouldClose(window)) {
        static long long previousFrame = getCurrentTime();
        auto frameStart = getCurrentTime();
        auto delta = frameStart - previousFrame;
        previousFrame = frameStart;

        glfwPollEvents();

        updateMovements(delta);
        renderFrame(actor);

        glfwSwapBuffers(window);

        fps++;

        auto frameEnd = getCurrentTime();
        if (frameEnd - start >= 1000) {
            std::cout << "Fps counter: " << fps << std::endl;
            fps = 0;
            start = frameEnd;
        }

        auto renderedBy = frameEnd - frameStart;
        long long int targetTime = (long long int) (1000.f /
                                                    (glfwGetWindowAttrib(window, GLFW_ICONIFIED) ? ICONIFIED_FPS_TARGET
                                                                                                 : DEFAULT_FPS_TARGET));
        if (renderedBy <= targetTime) {
            std::this_thread::sleep_for(std::chrono::nanoseconds((targetTime - renderedBy) * 1000));
        }
    }

    glfwTerminate();
    return 0;
}

void updateMovements(long long int delta) {
    const glm::vec3 &position = camera.getPosition();
    const glm::vec3 &front = camera.getFront();
    const glm::vec3 &up = camera.getUp();

    glm::vec3 newPosition = position;

    float cameraSpeed = delta / 1000.f * CAMERA_SPEED;

    if (pressedKeys[GLFW_KEY_W]) {
        newPosition += (cameraSpeed * front);
    }

    if (pressedKeys[GLFW_KEY_S]) {
        newPosition -= (cameraSpeed * front);
    }
    if (pressedKeys[GLFW_KEY_A]) {
        newPosition -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }

    if (pressedKeys[GLFW_KEY_D]) {
        newPosition += glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }

    camera.setPosition(newPosition);
}

void renderFrame(Actor *pActor) {
    glClearColor(0.4f, 0.4f, 0.4f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projectionViewMatrix = perspective * camera.getViewMatrix();
    pActor->update(projectionViewMatrix);
    pActor->render();
}