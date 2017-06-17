#include "Animator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

Animator::Animator() : animation(NULL), skeleton(NULL), playAnimation(false), currentTick(0) {
}

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

    auto ticksPerDelta = deltaTime / 1000.f * animation->getTicksPerSecond();
    float newTickValue = currentTick + ticksPerDelta;
    currentTick = fmodf(newTickValue, animation->getTotalTicksCount());

    updateBones(skeleton->getRootNode(), static_cast<int>(currentTick));
}

void Animator::updateBones(const aiNode *node, int deltaTime, glm::mat4 transformation) {
    auto bindMatrix = glm::toGlm(node->mTransformation);

    if (animation->containsBone(node->mName.data)) {
        auto translation = animation->getInterpolatedTranslation(node->mName.data, deltaTime);
        auto rotation = animation->getInterpolatedRotation(node->mName.data, deltaTime);
        auto scaling = animation->getInterpolatedScaling(node->mName.data, deltaTime);

        transformation =
                transformation * glm::translate(glm::mat4(1), translation) * glm::scale(glm::mat4(1), scaling) *
                glm::toMat4(rotation);
    } else {
        transformation = transformation * bindMatrix;
    }

    auto bone = skeleton->findBone(node->mName.data);

    if (bone != NULL) {
        bone->animatedBoneOffsetMatrix = transformation * bone->boneOffsetMatrix * glm::inverse(
                glm::transpose(glm::toGlm(skeleton->getRootNode()->mTransformation)));
        bone->animatedBoneMatrix = transformation;
    }

    for (auto i = 0; i < node->mNumChildren; i++) {
        updateBones(node->mChildren[i], deltaTime, transformation);
    }
}

void Animator::start() {
    playAnimation = true;
}

void Animator::stop() {
    playAnimation = false;
}
