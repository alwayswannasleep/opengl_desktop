#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include "glm.hpp"

class Camera {

public:

    glm::mat4 getViewMatrix();

    void setPosition(glm::vec3 position);

    void setTarget(glm::vec3 targetPosition);

    glm::vec3 getPosition();

    glm::vec3 getTarget();

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 upVector;
};


#endif //OPENGL_CAMERA_H
