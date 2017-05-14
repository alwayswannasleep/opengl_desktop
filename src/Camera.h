#pragma once

#include "glm.hpp"

class Camera {

public:

    Camera();

    glm::mat4 getViewMatrix();

    void setPosition(glm::vec3 position);

    glm::vec3 getPosition();

    glm::vec3 getFront();

    glm::vec3 getUp();

    void setFront(glm::vec3 newFront);

    void handleRotations(float yaw, float pitch);

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 upVector;

    float yawValue;
    float pitchValue;
};