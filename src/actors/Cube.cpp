#include "Cube.h"

Cube::Cube() : Model() {
    initialize("resources/models/cube.obj");
}

void Cube::initialize(const char *path) {
    Model::initialize(path);

    texture = new Texture();
    if (!texture->load("resources/models/texture.jpg")) {
        LOGM("Error loading texture for cube.");
    }
}

void Cube::render() {
    texture->bind(GL_TEXTURE0);

    Model::render();
}