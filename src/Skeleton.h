#pragma once

#include "glwrapper.h"
#include <string>
#include <unordered_map>
#include "glm.hpp"
#include <vector>

class Skeleton {
public:
    struct Bone {
        std::string boneName;
        glm::mat4 bindMatrix;
        glm::mat4 boneOffsetMatrix;
        std::vector<Bone *> children;
        Bone *parent;
    };

public:
    void insert(Bone *bone);

    Bone *findBone(std::string &boneName);

    GLint getBoneIndex(std::string &boneName);

    std::unordered_map<std::string, Bone *> &getBonesMap();

    std::unordered_map<std::string, GLint> &getBonesIndexes();

private:
    std::unordered_map<std::string, Bone *> bonesMap;
    std::unordered_map<std::string, GLint> bonesIndexes;
};