#include "glwrapper.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include <thread>
#include <glm/ext.hpp>
#include "Camera.h"
#include "actors/Actor.h"
#include "logs.h"
#include "assimp/Importer.hpp"
#include "actors/Model.h"
#include "actors/Cube.h"
#include "actors/ScreenPlane.h"

#define DEFAULT_FPS_TARGET 60
#define ICONIFIED_FPS_TARGET 5
#define CAMERA_SPEED 5;

Camera camera;
glm::mat4 perspective;
bool pressedKeys[1024];
Model *model;

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

    auto window = glfwCreateWindow(1600, 900, "OpenGL test", NULL, NULL);

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
    glEnable(GL_BLEND);

    glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    camera.setPosition(glm::vec3(0, 2, 4));

    Cube cube;
    cube.setScale(100, 0.001, 100);
    cube.setCamera(&camera);
    cube.setPosition(glm::vec3(0, 0, 0));
    model = new Model();
    model->setCamera(&camera);
    model->initialize("../resources/caster/caster.fbx");
    model->setScale(0.1, 0.1, 0.1);

    Model shark;
    shark.initialize("../resources/shark/shark.fbx");
    shark.setCamera(&camera);
    shark.setPosition(glm::vec3(-30, 10, 0));
    shark.setScale(0.09, 0.09, 0.09);

    Model sphere;
    sphere.initialize("../resources/monkey.fbx");
    sphere.setCamera(&camera);
    sphere.setRotation(-90, 0, 0);
    sphere.setScale(5, 5, 5);
    sphere.setPosition(glm::vec3(10, 10, 0));

    ScreenPlane screen;

    perspective = glm::perspective<float>(45, 16.f / 9.f, 0.01f, 1000);

    GLuint hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1600, 900, 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLuint hdrFrameBuffer;
    glGenFramebuffers(1, &hdrFrameBuffer);

    GLuint renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1600, 900);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGM("Error, cannot create frameBuffer.");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &hdrFrameBuffer);
        return -1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int fps = 0;
    auto start = getCurrentTime();
    while (!glfwWindowShouldClose(window)) {
        static long long previousFrame = getCurrentTime();
        auto frameStart = getCurrentTime();
        auto delta = frameStart - previousFrame;
        previousFrame = frameStart;

        glfwPollEvents();

        updateMovements(delta);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
        {
            glClearColor(0.01f, 0.01f, 0.01f, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto projectionViewMatrix = perspective * camera.getViewMatrix();

            cube.update(projectionViewMatrix, delta);
            cube.render();

            sphere.update(projectionViewMatrix, delta);
            sphere.render();

            model->update(projectionViewMatrix, delta);
            model->render();

            shark.update(projectionViewMatrix, delta);
            shark.render();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        screen.render();

        glBindTexture(GL_TEXTURE_2D, 0);

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

    if (pressedKeys[GLFW_KEY_SPACE]) {
        newPosition += (cameraSpeed * up);
    }

    if (pressedKeys[GLFW_KEY_LEFT_CONTROL]) {
        newPosition -= (cameraSpeed * up);
    }

    camera.setPosition(newPosition);
}