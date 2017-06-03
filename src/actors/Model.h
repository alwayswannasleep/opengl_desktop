#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include "Actor.h"
#include "assimp/Importer.hpp"
#include "glm.hpp"

class Model : public Actor {
public:
    Model() : Actor("../../shaders/model_vertex_shader.glsl", "../../shaders/model_fragment_shader.glsl") {}


private:
    Assimp::Importer importer;
    aiScene* scene;
};


#endif //OPENGL_MODEL_H
