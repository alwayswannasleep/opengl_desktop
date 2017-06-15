#include "Skeleton.h"
#include "logs.h"


void Skeleton::insert(Skeleton::Bone *bone) {
    if (bonesMap.count(bone->boneName) != 0) {
        return;
    }

    bonesMap.insert({bone->boneName, bone});
    bonesIndexes.insert({bone->boneName, bonesIndexes.size()});

    LOGI("Skeleton: bone '%s' with id - '%d'\n", bone->boneName.c_str(), bonesIndexes[bone->boneName]);
}

Skeleton::Bone *Skeleton::findBone(const std::string &boneName) const {
    if (bonesMap.count(boneName) == 0) {
        return NULL;
    }

    return bonesMap.at(boneName);
}

GLint Skeleton::getBoneIndex(const std::string &boneName) const {
    if (bonesIndexes.count(boneName) == 0) {
        return -1;
    }

    return bonesIndexes.at(boneName);
}

std::unordered_map<std::string, Skeleton::Bone *> &Skeleton::getBonesMap() {
    return bonesMap;
}

std::unordered_map<std::string, GLint> &Skeleton::getBonesIndexes() {
    return bonesIndexes;
}

void Skeleton::setRootNode(const aiNode *rootNode) {
    this->rootNode = rootNode;
}

const aiNode *Skeleton::getRootNode() const {
    return rootNode;
}
