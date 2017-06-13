#pragma once

#include "Animation.h"
#include "Skeleton.h"

class Animator {

public:

    void setSkeleton(Skeleton *skeleton);

    void setAnimation(Animation *animation);

    void update(int deltaTime);

    void start();

    void stop();

private:

    void updateBones(Skeleton::Bone *bone, int deltaTime);

private:

    Animation *animation;

    int currentTick;

    Skeleton *skeleton;

    bool playAnimation;
};