#ifndef OPENGL_PROGRAM_H
#define OPENGL_PROGRAM_H

#include "Gl/glew.h"

class Program {

public:

    Program(const char *vertexShaderPath, const char *fragmentShaderPath);

    ~Program();

public:
    void use();

private:
    GLuint compileShader(GLenum shaderType, const char *source);

    const char *loadShaderSourceFromFile(const char *path);

private:
    GLuint programId;
};


#endif
