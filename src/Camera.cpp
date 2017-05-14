#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Camera::Camera() : target(0, 0, -1), position(0, 0, 0), upVector(0, 1, 0), yawValue(0), pitchValue(0) {
}

void Camera::handleRotations(float yaw, float pitch) {
    yawValue += yaw;
    pitchValue += pitch;

    if (pitchValue > 89.0f) {
        pitchValue = 89.0f;
    } else if (pitchValue < -89.0f) {
        pitchValue = -89.0f;
    }

    glm::vec3 front;
    front.x = glm::cos(glm::radians(pitchValue)) * glm::cos(glm::radians(yawValue));
    front.y = glm::sin(glm::radians(pitchValue));
    front.z = glm::cos(glm::radians(pitchValue)) * glm::sin(glm::radians(yawValue));

    target = glm::normalize(front);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + target, upVector);
}

void Camera::setPosition(glm::vec3 position) {
    Camera::position = position;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getFront() {
    return target;
}

glm::vec3 Camera::getUp() {
    return upVector;
}

void Camera::setFront(glm::vec3 newFront) {
    target = newFront;
}
