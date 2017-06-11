#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "Actor.h"

class Cube : public Actor {
private:
    static const GLfloat VERTICES[];
    static const GLuint INDEXES[];

public:
    Cube();


    void render() override;

private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};


#endif //OPENGL_CUBE_H
