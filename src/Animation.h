#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "assimp/anim.h"
#include <unordered_map>
#include "glm_utils.h"

class Animation {

public:
    void initialize(const aiAnimation *animation);

    int getTotalTicksCount();

    int getTicksPerSecond();

    glm::vec3 getInterpolatedTranlsation(std::string &nodeName, float animationTime);

    glm::vec3 getInterpolatedScaling(std::string &nodeName, float animationTime);

    glm::quat getInterpolatedRotation(std::string &nodeName, float animationTime);

    std::string &getName();

private:
    int totalTicksCount;
    int ticksPerSecond;

    std::string name;

    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> translationsIndexes;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> scalingIndexes;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> rotationIndexes;
    std::unordered_map<std::string, std::vector<aiVectorKey>> translations;
    std::unordered_map<std::string, std::vector<aiVectorKey>> scaling;
    std::unordered_map<std::string, std::vector<aiQuatKey>> rotations;
};