#pragma once

#include "Actor.h"

class ScreenPlane : public Actor {
private:
    static const float VERTICES[];

public:
    ScreenPlane();

    void render() override;

private:

    GLuint vertexArrayObject;
    GLuint vertexBufferObject;
};
