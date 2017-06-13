#include "Skeleton.h"


void Skeleton::insert(Skeleton::Bone *bone) {
    if (bonesMap.count(bone->boneName) != 0) {
        return;
    }

    bonesMap.insert({bone->boneName, bone});
    bonesIndexes.insert({bone->boneName, bonesIndexes.size() + 1});
}

Skeleton::Bone *Skeleton::findBone(std::string &boneName) {
    if (bonesMap.count(boneName) == 0) {
        return NULL;
    }

    return bonesMap[boneName];
}

GLint Skeleton::getBoneIndex(std::string &boneName) {
    if (bonesIndexes.count(boneName) == 0) {
        return -1;
    }

    return bonesIndexes[boneName];
}

std::unordered_map<std::string, Skeleton::Bone *> &Skeleton::getBonesMap() {
    return bonesMap;
}

std::unordered_map<std::string, GLint> &Skeleton::getBonesIndexes() {
    return bonesIndexes;
}

void Skeleton::setRootBone(Skeleton::Bone *bone) {
    rootBone = bone;
}

Skeleton::Bone *Skeleton::getRootBone() {
    return rootBone;
}
