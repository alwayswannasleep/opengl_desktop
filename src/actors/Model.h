#pragma once

#include "Actor.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "glm.hpp"
#include "../Texture.h"
#include "../logs.h"
#include "../glm_utils.h"

class Model : public Actor {

    struct Material {
        glm::vec3 specularColor;
        glm::vec3 diffuseColor;
        glm::vec3 ambientColor;
        float shininess;
        float shininessStrength;
        float reflectivity;
        bool useWireframeRendering;
        bool disableBackCulling;
        Texture *texture;
        bool hasTexture;

        Material();

        ~Material();

        bool setTexture(const std::string &textureFile);

        void setTexture(const aiTexture *texture);
    };


    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
    };

    struct Mesh {
        GLuint vertexArrayObject;
        unsigned int materialIndex;
        size_t indexesCount;

        void init(std::vector<Vertex> &vertices, std::vector<unsigned int> &indexes);

        void release();
    };

    struct Node {
        std::vector<Mesh> meshes;
        std::string nodeName;
        glm::mat4 globalTransformation;

        void copyMeshes(const aiScene *scene, aiNode *node);

        void release();
    };

public:
    Model() : Actor("../shaders/model_vertex_shader.glsl", "../shaders/model_fragment_shader.glsl") {}

    void initialize(const char *path);

    void render() override;

private:

    void processNodes(aiNode *node, glm::mat4 transformation = glm::mat4(1));

    void initializeMaterials(const std::string &modelFile);

private:
    Assimp::Importer importer;
    const aiScene *scene;

    std::vector<Node> nodes;
    std::vector<Material *> materials;
};