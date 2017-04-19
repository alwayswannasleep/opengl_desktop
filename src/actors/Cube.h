#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "Actor.h"

class Cube : public Actor {
private:
    static const GLfloat VERTICES[];
    static const GLuint INDEXES[];
    static const float TEXTURE_COORDS[];

public:
    Cube();


    void render() override;

private:
    GLuint vbo;
    GLuint ebo;
};


#endif //OPENGL_CUBE_H
