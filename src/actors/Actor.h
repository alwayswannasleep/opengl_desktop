#ifndef OPENGL_ACTOR_H
#define OPENGL_ACTOR_H

#include "../glwrapper.h"
#include "glm/glm.hpp"
#include "../Program.h"
#include "../Camera.h"
#include <vector>

class Actor {

public:
    ~Actor();

    virtual void update(glm::mat4 &transformationMatrix, float deltaTime);

    virtual void render();

    void addChild(Actor &actor);

    void setPosition(glm::vec3 position);

    void setRotation(float x, float y, float z);

    void setScale(float x, float y, float z);

    glm::vec3 getPosition();

    void setCamera(Camera *camera);

    virtual void release();

protected:

    Actor(const char *vertexPath, const char *fragmentPath);

    bool isChild();

protected:

    Program *program;
    Camera *camera;

    glm::mat4 projectionViewMatrix;
    glm::mat4 modelMatrix;
    std::vector<Actor *> children;
    glm::vec3 position;
    glm::mat4 translationMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;

    bool isNestedChild;
};


#endif //OPENGL_ACTOR_H
