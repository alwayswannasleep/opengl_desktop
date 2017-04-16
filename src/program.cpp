#include <sstream>
#include <fstream>
#include <iostream>
#include "program.h"

Program::Program(const char *vertexShaderPath, const char *fragmentShaderPath) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, loadShaderSourceFromFile(vertexShaderPath));
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, loadShaderSourceFromFile(fragmentShaderPath));

    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetProgramInfoLog(programId, sizeof(info), NULL, info);
        std::cerr << "ERROR::PROGRAM LINK_FAILED\n" << info << std::endl;

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
        std::cerr << "ERROR::SHADER COMPILE_FAILED\n" << info << std::endl;
        return 0;
    }

    return id;
}

const char *Program::loadShaderSourceFromFile(const char *path) {
    std::string source;
    std::ifstream file(path);

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream sourceStream;
        sourceStream << file.rdbuf();
        source = sourceStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return source.c_str();
}
