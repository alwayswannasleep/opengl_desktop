#ifndef OPENGL_ACTOR_H
#define OPENGL_ACTOR_H

#include "../glwrapper.h"
#include "glm/glm.hpp"
#include "../Program.h"
#include <vector>

class Actor {

public:
    ~Actor();

    void update(glm::mat4 &transformationMatrix);

    virtual void render();

    bool isChild();

    void addChild(Actor &actor);

    void setPosition(glm::vec3 position);

    void setRotation(float x, float y, float z);

    void setScale(float x, float y, float z);

    glm::vec3 getPosition();

protected:

    Actor(const char *vertexPath, const char *fragmentPath);

protected:

    GLuint vao;
    Program *program;
    glm::mat4 modelMatrix;
    std::vector<Actor *> children;
    glm::vec3 position;
    glm::mat4 translationMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;

};


#endif //OPENGL_ACTOR_H
