#pragma once

#include "Animation.h"
#include "Skeleton.h"

class Animator {

public:

    Animator();

    void setSkeleton(Skeleton *skeleton);

    void setAnimation(Animation *animation);

    void update(int deltaTime);

    void start();

    void stop();

private:

    void updateBones(const aiNode *node, int deltaTime, glm::mat4 transformation = glm::mat4(1));

private:

    Animation *animation;

    float currentTick;

    Skeleton *skeleton;

    bool playAnimation;
};