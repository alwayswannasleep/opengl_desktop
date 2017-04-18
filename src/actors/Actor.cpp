#include "Actor.h"
#include "gtc/matrix_transform.hpp"

Actor::Actor(const char *vertexPath, const char *fragmentPath) :
        program(vertexPath, fragmentPath),
        rotationMatrix(1),
        scaleMatrix(1),
        translationMatrix(1) {}

Actor::~Actor() {
    for (auto actor : children) {
        delete actor;
    }
}

void Actor::update(glm::mat4 transformationMatrix) {
    modelMatrix = transformationMatrix/* * translationMatrix * scaleMatrix * rotationMatrix*/;

    for (auto child : children) {
        child->update(modelMatrix);
    }
}

void Actor::render() {
    for (auto child : children) {
        child->render();
    }
}

bool Actor::isChild() {
    return false;
}

void Actor::addChild(Actor &actor) {
    children.push_back(&actor);
}

void Actor::setPosition(glm::vec3 position) {
    Actor::position = position;
    translationMatrix = glm::translate(glm::mat4(1), position);
}

void Actor::setRotation(float x, float y, float z) {
    glm::rotate(rotationMatrix, x, {1, 0, 0});
    glm::rotate(rotationMatrix, y, {0, 1, 0});
    glm::rotate(rotationMatrix, z, {0, 0, 1});
}

void Actor::setScale(float x, float y, float z) {
    scaleMatrix = glm::scale(glm::mat4(1), {x, y, z});
}

glm::vec3 Actor::getPosition() {
    return glm::vec3();
}
