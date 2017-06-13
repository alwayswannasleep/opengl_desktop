#include "Animator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Animator::setSkeleton(Skeleton *skeleton) {
    this->skeleton = skeleton;
}

void Animator::setAnimation(Animation *animation) {
    this->animation = animation;
}

void Animator::update(int deltaTime) {
    if (animation == NULL || !playAnimation) {
        return;
    }

    currentTick = static_cast<int>(std::fmod(deltaTime / 1000.f * animation->getTicksPerSecond(),
                                             animation->getTotalTicksCount()));

    auto rootBone = skeleton->getRootBone();
    updateBones(rootBone, currentTick);
}

void Animator::updateBones(Skeleton::Bone *rootBone, int deltaTime) {
    auto bindMatrix = rootBone->bindMatrix;

    glm::mat4 parentAnimatedMatrix(1);

    if (rootBone->parent != NULL) {
        parentAnimatedMatrix = rootBone->parent->animatedBoneMatrix;
    }

    if (animation->containsBone(rootBone->boneName)) {
        auto translation = animation->getInterpolatedTranslation(rootBone->boneName, deltaTime);
        auto rotation = animation->getInterpolatedRotation(rootBone->boneName, deltaTime);
        auto scaling = animation->getInterpolatedScaling(rootBone->boneName, deltaTime);

        rootBone->animatedBoneMatrix =
                parentAnimatedMatrix * glm::translate(glm::mat4(1), translation) * glm::scale(glm::mat4(1), scaling) *
                glm::toMat4(rotation);
        rootBone->animatedBoneOffsetMatrix = rootBone->animatedBoneMatrix * rootBone->boneOffsetMatrix;
    } else {
        rootBone->animatedBoneMatrix = parentAnimatedMatrix * bindMatrix;
    }

    for (auto bone : rootBone->children) {
        updateBones(bone, deltaTime);
    }
}

void Animator::start() {
    playAnimation = true;
}

void Animator::stop() {
    playAnimation = false;
}
