#include "Cube.h"

Cube::Cube() : Model() {
    initialize("../resources/cube.obj");
}

void Cube::initialize(const char *path) {
    Model::initialize(path);

    texture = new Texture();
    if (!texture->load("../resources/texture.jpg")) {
        LOGM("Error loading texture for cube.");
    }
}

void Cube::render() {
    texture->bind(GL_TEXTURE0);

    Model::render();
}