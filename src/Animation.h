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

    int getTotalTicksCount() const;

    int getTicksPerSecond() const;

    glm::vec3 getInterpolatedTranslation(const std::string &nodeName, const float &animationTime) const;

    glm::vec3 getInterpolatedScaling(const std::string &nodeName, const float &animationTime) const;

    glm::quat getInterpolatedRotation(const std::string &nodeName, const float &animationTime) const;

    const std::string &getName() const;

    bool containsBone(const std::string &boneName) const;

private:
    int totalTicksCount;
    int ticksPerSecond;

    std::string name;

    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> translationsIndexes;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> scalingIndexes;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> rotationIndexes;
    std::unordered_map<std::string, std::vector<aiVectorKey>> translations;
    std::unordered_map<std::string, std::vector<aiVectorKey>> scalings;
    std::unordered_map<std::string, std::vector<aiQuatKey>> rotations;
};