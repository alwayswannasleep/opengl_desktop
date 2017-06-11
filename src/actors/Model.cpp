#include <glm/ext.hpp>
#include "Model.h"

void Model::Mesh::init(std::vector<Vertex> &vertices, std::vector<unsigned int> &indexes) {
    glGenVertexArrays(1, &vertexArrayObject);

    GLuint vbo;
    GLuint ebo;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<GLvoid *>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<GLvoid *>(offsetof(Vertex, textureCoordinates)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes[0]) * indexes.size(), &indexes[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    indexesCount = indexes.size();
}

void Model::Mesh::release() {
    glDeleteVertexArrays(1, &vertexArrayObject);
}

Model::Material::Material() :
        texture(NULL),
        ambientColor(0, 0, 0),
        diffuseColor(0.8f, 0.8f, 0.8f),
        specularColor(0, 0, 0),
        shininess(0),
        shininessStrength(0),
        disableBackCulling(false),
        useWireframeRendering(false),
        hasTexture(false),
        reflectivity(0) {}

Model::Material::~Material() {
    delete texture;
}

bool Model::Material::setTexture(const std::string &textureFile) {
    texture = new Texture();
    hasTexture = texture->load(textureFile);

    if (!hasTexture) {
        delete texture;
        texture = NULL;
    }

    return hasTexture;
}

void Model::Material::setTexture(const aiTexture *importedTexture) {
    if (importedTexture->mHeight == 0) {
        auto dataPointer = reinterpret_cast<unsigned char *>(importedTexture->pcData);

        texture = new Texture();
        hasTexture = texture->load(dataPointer, importedTexture->mWidth);
        return;
    }

    texture = new Texture();
    hasTexture = texture
            ->load(importedTexture->pcData, importedTexture->mWidth * importedTexture->mHeight);
}


void Model::Node::copyMeshes(const aiScene *scene, aiNode *node) {
    auto meshesNumber = node->mNumMeshes;

    if (meshesNumber == 0) {
        return;
    }

    meshes.resize(meshesNumber);

    for (int i = 0; i < meshesNumber; i++) {
        auto meshIndex = node->mMeshes[i];
        auto importedMesh = scene->mMeshes[meshIndex];
        auto &meshEntry = meshes[i];

        meshEntry.materialIndex = importedMesh->mMaterialIndex;

        auto verticesCount = importedMesh->mNumVertices;
        std::vector<Vertex> vertices;
        vertices.reserve(verticesCount);

        const aiVector3D zeros(0, 0, 0);

        for (int j = 0; j < verticesCount; j++) {
            auto pos = importedMesh->mVertices[j];
            auto normal = importedMesh->mNormals[j];
            auto textureCoordinates = importedMesh->HasTextureCoords(0)
                                      ? importedMesh->mTextureCoords[0][j] : zeros;
            Vertex vertex;
            vertex.position = glm::toGlm(pos);
            vertex.normal = glm::toGlm(normal);
            vertex.textureCoordinates = glm::toGlm(textureCoordinates);
            vertices.push_back(vertex);
        }

        auto facesCount = importedMesh->mNumFaces;
        std::vector<unsigned int> indices;
        indices.reserve(facesCount * 3);

        for (int j = 0; j < facesCount; j++) {
            auto face = importedMesh->mFaces[j];

            for (int k = 0; k < 3; k++) {
                indices.push_back(face.mIndices[k]);
            }
        }

        meshEntry.init(vertices, indices);
    }
}

void Model::Node::release() {

}

void Model::initialize(const char *path) {
    scene = importer.ReadFile(path, aiProcess_Triangulate |
                                    aiProcess_FlipUVs |
                                    aiProcess_GenSmoothNormals |
                                    aiProcess_SplitLargeMeshes |
                                    aiProcess_JoinIdenticalVertices);

    if (scene == NULL) {
        LOGI("Error: error loading model: %s.\n", path);
        return;
    }

    initializeMaterials(path);

    program->use();

    processNodes(scene->mRootNode);
}

void Model::processNodes(aiNode *node, glm::mat4 transformation) {
    auto tmp = transformation * glm::toGlm(node->mTransformation);

    if (node->mNumMeshes > 0) {
        Node object;

        object.nodeName = node->mName.data;
        object.globalTransformation = tmp;

        object.copyMeshes(scene, node);

        nodes.push_back(object);
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        processNodes(node->mChildren[i], tmp);
    }
}

void Model::initializeMaterials(const std::string &modelFile) {
    if (!scene->HasMaterials()) {
        LOGI("Model '%s' doesn't have materials.\n", modelFile.c_str());
        return;
    }

    materials.resize(scene->mNumMaterials);

    auto directory = modelFile;
    auto slashIndex = directory.find_last_of('/');
    if (slashIndex == std::string::npos) {
        directory = ".";
    } else if (slashIndex == 0) {
        directory = "/";
    } else {
        directory = directory.substr(0, slashIndex);
    }

    for (int i = 0; i < scene->mNumMaterials; i++) {

        auto assimpMaterial = scene->mMaterials[i];

        materials[i] = new Material;
        aiString texturePath;
        auto textureCount = assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE);
        if (textureCount != 0 &&
            assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
            std::string path(texturePath.data);
            std::string possibleNumber = path.substr(1);

            if (scene->HasTextures() && !possibleNumber.empty() &&
                possibleNumber.find_first_not_of("0123456789") == std::string::npos) {
                auto textureIndex = std::atoi(path.substr(1).c_str());
                auto texture = scene->mTextures[textureIndex];

                materials[i]->setTexture(texture);
            } else {
                if (path.substr(0, 2) == ".\\") {
                    path = path.substr(2, path.size() - 2);
                }

                std::replace(path.begin(), path.end(), '\\', '/');
                std::string fileName = "";
                slashIndex = path.find_last_of("/");
                if (slashIndex == std::string::npos) {
                    fileName = path;
                } else {
                    fileName = path.substr(slashIndex + 1);
                }
                std::vector<std::string> paths;
                paths.reserve(4);
                paths.push_back(directory + "/" + path);
                paths.push_back(directory + "/" + fileName);
                paths.push_back("/" + fileName);

                for (int index = 0; !materials[i]->hasTexture && index < paths.size(); index++) {
                    std::string FullPath = paths[index];

                    if (!materials[i]->setTexture(FullPath)) {
                        LOGI("Error loading texture '%s'\n", FullPath.c_str());

                        continue;
                    }

                    LOGI("%d - loaded texture '%s'\n", i, FullPath.c_str());
                }
            }
        }

        aiColor3D tmpColor;
        assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, tmpColor);
        materials[i]->diffuseColor = glm::toGlm(tmpColor);

        assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, tmpColor);
        materials[i]->ambientColor = glm::toGlm(tmpColor);

        assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, tmpColor);
        materials[i]->specularColor = glm::toGlm(tmpColor);

        assimpMaterial->Get(AI_MATKEY_SHININESS, materials[i]->shininess);
        assimpMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, materials[i]->shininessStrength);
        assimpMaterial->Get(AI_MATKEY_ENABLE_WIREFRAME, materials[i]->useWireframeRendering);
        assimpMaterial->Get(AI_MATKEY_REFLECTIVITY, materials[i]->reflectivity);
        assimpMaterial->Get(AI_MATKEY_TWOSIDED, materials[i]->disableBackCulling);
    }
}

void Model::render() {
    Actor::render();

    program->use();

    for (auto node : nodes) {
        auto meshes = node.meshes;
        for (auto mesh : meshes) {
            glUniformMatrix4fv(program->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

            if (materials[mesh.materialIndex]->hasTexture) {
                materials[mesh.materialIndex]->texture->bind(GL_TEXTURE0);
                glUniform1i(program->getUniformLocation("uTexture"), 0);
            };

            glBindVertexArray(mesh.vertexArrayObject);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexesCount), GL_UNSIGNED_INT, 0);
        }
    }

    glBindVertexArray(0);
}