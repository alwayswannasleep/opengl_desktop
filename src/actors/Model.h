#pragma once

#include <unordered_map>
#include "Actor.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "glm.hpp"
#include "../Texture.h"
#include "../logs.h"
#include "../glm_utils.h"
#include "../Skeleton.h"
#include "../Animation.h"
#include "../Animator.h"

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
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 textureCoordinates = glm::vec2(0.0f);
        glm::ivec4 bonesIDs = glm::vec4(0.0f);
        glm::vec4 bonesWeights = glm::vec4(0.0f);

        void tryAddBoneData(GLint boneId, GLfloat weight);
    };

    struct Mesh {
        GLuint vertexArrayObject;
        unsigned int materialIndex;
        size_t indexesCount;
        GLboolean hasBones;

        void init(std::vector<Vertex> &vertices, std::vector<unsigned int> &indexes);

        void release();
    };

    struct Node {
        std::vector<Mesh> meshes;
        std::string nodeName;
        glm::mat4 globalTransformation;

        void copyMeshes(const aiScene *scene, aiNode *node, Skeleton &skeleton);

        void release();
    };

public:
    Model() : Actor("../shaders/model_vertex_shader.glsl", "../shaders/model_fragment_shader.glsl") {}

    void initialize(const char *path);

    void update(glm::mat4 &transformationMatrix, float deltaTime) override;

    void render() override;

    void release() override;

private:

    void processNodes(aiNode *node, glm::mat4 transformation = glm::mat4(1));

    void initializeMaterials(const std::string &modelFile);

private:
    Assimp::Importer importer;
    const aiScene *scene;

    Animator animator;

    Skeleton skeleton;
    std::vector<Node> nodes;
    std::vector<Material *> materials;
    std::vector<Animation *> animations;
};