#include <gtc/matrix_transform.hpp>
#include "Camera.h"

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, target, {0, 1, 0});
}

void Camera::setPosition(glm::vec3 position) {
    Camera::position = position;
}

void Camera::setTarget(glm::vec3 targetPosition) {
    Camera::target = targetPosition;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getTarget() {
    return target;
}
