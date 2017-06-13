#include "Animation.h"
#include "logs.h"

void Animation::initialize(const aiAnimation *animation) {
    name = animation->mName.data;
    ticksPerSecond = animation->mTicksPerSecond == 0 ? 30 : static_cast<int>(animation->mTicksPerSecond);
    totalTicksCount = static_cast<int>(animation->mDuration);

    auto channelsCount = animation->mNumChannels;
//
//    translations.reserve(channelsCount);
//    rotations.reserve(channelsCount);
//    scalings.reserve(channelsCount);
//
//    translationsIndexes.reserve(channelsCount);
//    rotationIndexes.reserve(channelsCount);
//    scalingIndexes.reserve(channelsCount);

    for (auto i = 0; i < channelsCount; i++) {
        auto channel = animation->mChannels[i];
        std::string channelName = channel->mNodeName.data;

        translations.emplace(channelName, std::vector<aiVectorKey>());
        translationsIndexes.emplace(channelName, std::vector<std::pair<size_t, size_t>>());
//
//        translations.at(channelName).resize(channel->mNumPositionKeys);
//        translationsIndexes.at(channelName).resize(static_cast<size_t>(totalTicksCount));

        size_t startPosition = 0;
        for (size_t j = 0; j < channel->mNumPositionKeys; j++) {
            auto key = channel->mPositionKeys[j];

            translations[channelName].push_back(key);

            if (j == 0) {
                continue;
            }

            for (auto k = startPosition; k <= key.mTime; k++) {
                translationsIndexes[channelName].push_back(std::make_pair(j - 1, j));
            }

            startPosition = static_cast<size_t>(key.mTime + 1);
        }

        rotations.emplace(channelName, std::vector<aiQuatKey>());
        rotationIndexes.emplace(channelName, std::vector<std::pair<size_t, size_t>>());
//
//        rotations.at(channelName).resize(channel->mNumRotationKeys);
//        rotationIndexes.at(channelName).resize(static_cast<size_t>(totalTicksCount));

        startPosition = 0;
        for (size_t j = 0; j < channel->mNumRotationKeys; j++) {
            auto key = channel->mRotationKeys[j];

            rotations[channelName].push_back(key);

            if (j == 0) {
                continue;
            }

            for (auto k = startPosition; k <= key.mTime; k++) {
                rotationIndexes[channelName].push_back(std::make_pair(j - 1, j));
            }

            startPosition = static_cast<size_t>(key.mTime + 1);
        }

        scalings.emplace(channelName, std::vector<aiVectorKey>());
        scalingIndexes.emplace(channelName, std::vector<std::pair<size_t, size_t>>());
//
//        scalings.at(channelName).resize(channel->mNumScalingKeys);
//        scalingIndexes.at(channelName).resize(static_cast<size_t>(totalTicksCount));

        startPosition = 0;
        for (size_t j = 0; j < channel->mNumScalingKeys; j++) {
            auto key = channel->mScalingKeys[j];

            scalings[channelName].push_back(key);

            if (j == 0) {
                continue;
            }

            for (auto k = startPosition; k <= key.mTime; k++) {
                scalingIndexes[channelName].push_back(std::make_pair(j - 1, j));
            }

            startPosition = static_cast<size_t>(key.mTime + 1);
        }
    }

    LOGI("Animation: scaling count %ld\n", scalings.size());
    LOGI("Animation: rotation count %ld\n", rotations.size());
    LOGI("Animation: translation count %ld\n", translations.size());
}

int Animation::getTotalTicksCount() const {
    return totalTicksCount;
}

int Animation::getTicksPerSecond() const {
    return ticksPerSecond;
}

glm::vec3 Animation::getInterpolatedTranslation(const std::string &nodeName, const float &animationTime) const {
    if (translationsIndexes.count(nodeName) == 0) {
        return glm::vec3(-1);
    }

    if (translations.at(nodeName).size() == 1) {
        return glm::toGlm(translations.at(nodeName).begin()->mValue);
    }

    auto indexes = translationsIndexes.at(nodeName).at(static_cast<size_t>(animationTime));
    auto start = translations.at(nodeName).at(indexes.first);
    auto end = translations.at(nodeName).at(indexes.second);

    auto keysDelta = end.mTime - start.mTime;
    auto delta = animationTime - start.mTime;
    float factor = std::abs(static_cast<float>(delta / keysDelta));

    return glm::toGlm((start.mValue - end.mValue) * factor + start.mValue);
}

glm::vec3 Animation::getInterpolatedScaling(const std::string &nodeName, const float &animationTime) const {
    if (scalingIndexes.count(nodeName) == 0) {
        return glm::vec3(-1);
    }

    if (scalings.at(nodeName).size() == 1) {
        return glm::toGlm(translations.at(nodeName).begin()->mValue);
    }

    auto indexes = scalingIndexes.at(nodeName).at(static_cast<size_t>(animationTime));
    auto start = scalings.at(nodeName).at(indexes.first);
    auto end = scalings.at(nodeName).at(indexes.second);

    auto keysDelta = end.mTime - start.mTime;
    auto delta = animationTime - start.mTime;
    float factor = std::abs(static_cast<float>(delta / keysDelta));

    return glm::toGlm((start.mValue - end.mValue) * factor + start.mValue);
}

glm::quat Animation::getInterpolatedRotation(const std::string &nodeName, const float &animationTime) const {
    if (rotationIndexes.count(nodeName) == 0) {
        return glm::quat(-1, -1, -1, -1);
    }

    if (rotationIndexes.at(nodeName).size() == 1) {
        return glm::toGlm(rotations.at(nodeName).begin()->mValue);
    }

    auto indexes = rotationIndexes.at(nodeName).at(static_cast<size_t>(animationTime));
    auto start = rotations.at(nodeName).at(indexes.first);
    auto end = rotations.at(nodeName).at(indexes.second);

    auto keysDelta = end.mTime - start.mTime;
    auto delta = animationTime - start.mTime;
    float factor = std::abs(static_cast<float>(delta / keysDelta));

    aiQuaternion result;
    aiQuaternion::Interpolate(result, start.mValue, end.mValue, factor);

    return glm::toGlm(result);
}

const std::string &Animation::getName() const {
    return name;
}

bool Animation::containsBone(const std::string &boneName) const {
    return rotations.count(boneName) != 0 && translations.count(boneName) != 0 && scalings.count(boneName) != 0;
}