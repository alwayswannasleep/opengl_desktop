#ifndef OPENGL_PROGRAM_H
#define OPENGL_PROGRAM_H

#include "glwrapper.h"
#include "string"

class Program {

public:

    Program(const char *vertexShaderPath, const char *fragmentShaderPath);

    ~Program();

    void release();

public:
    void use();

    GLint getUniformLocation(const char *name);

    GLint getAttributeLocation(const char *name);

private:
    GLuint compileShader(GLenum shaderType, const char *source);

    std::string loadShaderSourceFromFile(const char *path);

private:
    GLuint programId;
};


#endif
