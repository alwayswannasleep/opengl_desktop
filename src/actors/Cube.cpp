#include "Cube.h"

Cube::Cube() : Model() {
    initialize("../resources/cube.obj");
}

void Cube::initialize(const char *path) {
    Model::initialize(path);
}
