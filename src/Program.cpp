#include <sstream>
#include <fstream>
#include <iostream>
#include "Program.h"
#include "logs.h"

Program::Program(const char *vertexShaderPath, const char *fragmentShaderPath) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, loadShaderSourceFromFile(vertexShaderPath).c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, loadShaderSourceFromFile(fragmentShaderPath).c_str());

    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetProgramInfoLog(programId, sizeof(info), NULL, info);
        LOGI("ERROR::PROGRAM LINK_FAILED\n%s", info);

        glDeleteProgram(programId);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Program::~Program() {
    glDeleteProgram(programId);
}

void Program::use() {
    glUseProgram(programId);
}

GLuint Program::compileShader(GLenum shaderType, const char *source) {
    GLuint id = glCreateShader(shaderType);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetShaderInfoLog(id, sizeof(info), NULL, info);
        LOGI("ERROR::SHADER LINK_FAILED\n%s", info);
        return 0;
    }

    return id;
}

std::string Program::loadShaderSourceFromFile(const char *path) {
    std::string shaderCode;
    std::ifstream shaderStream(path, std::ios::in);
    if (shaderStream.is_open()) {
        std::string line = "";
        while (getline(shaderStream, line))
            shaderCode += "\n" + line;
        shaderStream.close();
    }

    return shaderCode;
}

GLint Program::getUniformLocation(const char *name) {
    return glGetUniformLocation(programId, name);
}

GLint Program::getAttributeLocation(const char *name) {
    return glGetAttribLocation(programId, name);
}

void Program::release() {
    glDeleteProgram(programId);
}