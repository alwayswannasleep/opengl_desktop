#pragma once

#include "glwrapper.h"
#include <string>
#include <unordered_map>
#include "glm.hpp"
#include <vector>
#include <assimp/scene.h>

class Skeleton {
public:
    struct Bone {
        std::string boneName;
        glm::mat4 boneOffsetMatrix;
        glm::mat4 animatedBoneMatrix = glm::mat4(1);
        glm::mat4 animatedBoneOffsetMatrix = glm::mat4(1);
    };

public:
    void insert(Bone *bone);

    Bone *findBone(const std::string &boneName) const;

    GLint getBoneIndex(const std::string &boneName) const;

    std::unordered_map<std::string, Bone *> &getBonesMap();

    std::unordered_map<std::string, GLint> &getBonesIndexes();

    void setRootNode(const aiNode *rootNode);

    const aiNode *getRootNode() const;

private:
    std::unordered_map<std::string, Bone *> bonesMap;
    std::unordered_map<std::string, GLint> bonesIndexes;
    const aiNode *rootNode;
};