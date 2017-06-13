#include "Animation.h"
#include "logs.h"

void Animation::initialize(const aiAnimation *animation) {
    name = animation->mName.data;
    ticksPerSecond = animation->mTicksPerSecond == 0 ? 30 : static_cast<int>(animation->mTicksPerSecond);
    totalTicksCount = static_cast<int>(animation->mDuration);

    auto channelsCount = animation->mNumChannels;

    translations.reserve(channelsCount);
    rotations.reserve(channelsCount);
    scaling.reserve(channelsCount);

    translationsIndexes.reserve(channelsCount);
    rotationIndexes.reserve(channelsCount);
    scalingIndexes.reserve(channelsCount);

    for (auto i = 0; i < channelsCount; i++) {
        auto channel = animation->mChannels[i];
        std::string channelName(channel->mNodeName.data);

        std::vector<std::pair<size_t, size_t>> positionKeys;
        positionKeys.resize(static_cast<size_t>(totalTicksCount));

        std::vector<aiVectorKey> positions;
        positions.resize(channel->mNumPositionKeys);

        size_t startPosition = 0;
        for (size_t j = 0; j < channel->mNumPositionKeys; j++) {
            auto key = channel->mPositionKeys[j];

            positions[j] = key;

            if (j == 0) {
                continue;
            }

            for (auto k = startPosition; k <= key.mTime; k++) {
                positionKeys[k] = {j - 1, j};
            }

            startPosition = static_cast<size_t>(key.mTime + 1);
        }

        translations[channelName] = positions;
        translationsIndexes[channelName] = positionKeys;

        std::vector<std::pair<size_t, size_t>> rotationsKeys;
        rotationsKeys.resize(static_cast<size_t>(totalTicksCount));

        std::vector<aiQuatKey> rotations;
        rotations.resize(channel->mNumRotationKeys);

        startPosition = 0;
        for (size_t j = 0; j < channel->mNumRotationKeys; j++) {
            auto key = channel->mRotationKeys[j];

            rotations[j] = key;

            if (j == 0) {
                continue;
            }

            for (auto k = startPosition; k <= key.mTime; k++) {
                positionKeys[k] = {j - 1, j};
            }

            startPosition = static_cast<size_t>(key.mTime + 1);
        }

        this->rotations[channelName] = rotations;
        rotationIndexes[channelName] = rotationsKeys;

        std::vector<std::pair<size_t, size_t>> scalingKeys;
        scalingKeys.resize(static_cast<size_t>(totalTicksCount));

        std::vector<aiVectorKey> scalings;
        scalings.resize(channel->mNumScalingKeys);

        startPosition = 0;
        for (size_t j = 0; j < channel->mNumScalingKeys; j++) {
            auto key = channel->mScalingKeys[j];

            scalings[j] = key;

            if (j == 0) {
                continue;
            }

            for (auto k = startPosition; k <= key.mTime; k++) {
                positionKeys[k] = {j - 1, j};
            }

            startPosition = static_cast<size_t>(key.mTime + 1);
        }

        scaling[channelName] = positions;
        scalingIndexes[channelName] = scalingKeys;
    }

    LOGI("Animation: scaling indexes count %ld\n", scalingIndexes.size());
    LOGI("Animation: rotation indexes count %ld\n", rotationIndexes.size());
    LOGI("Animation: translation indexes count %ld\n", translationsIndexes.size());
    LOGI("Animation: scaling count %ld\n", scaling.size());
    LOGI("Animation: rotation count %ld\n", rotations.size());
    LOGI("Animation: translation count %ld\n", translations.size());
}

int Animation::getTotalTicksCount() {
    return totalTicksCount;
}

int Animation::getTicksPerSecond() {
    return ticksPerSecond;
}

glm::vec3 Animation::getInterpolatedTranlsation(std::string &nodeName, float animationTime) {
    if (translationsIndexes.count(nodeName) == 0) {
        return glm::vec3(-1);
    }

    if (translations[nodeName].size() == 1) {
        return glm::toGlm(translations[nodeName].begin()->mValue);
    }

    auto indexes = translationsIndexes[nodeName][static_cast<size_t>(animationTime)];
    auto start = translations[nodeName][indexes.first];
    auto end = translations[nodeName][indexes.second];

    auto keysDelta = end.mTime - start.mTime;
    auto delta = animationTime - start.mTime;
    float factor = std::abs(static_cast<float>(delta / keysDelta));

    return glm::toGlm((start.mValue - end.mValue) * factor + start.mValue);
}

glm::vec3 Animation::getInterpolatedScaling(std::string &nodeName, float animationTime) {
    if (scalingIndexes.count(nodeName) == 0) {
        return glm::vec3(-1);
    }

    if (scaling[nodeName].size() == 1) {
        return glm::toGlm(translations[nodeName].begin()->mValue);
    }

    auto indexes = scalingIndexes[nodeName][static_cast<size_t>(animationTime)];
    auto start = scaling[nodeName][indexes.first];
    auto end = scaling[nodeName][indexes.second];

    auto keysDelta = end.mTime - start.mTime;
    auto delta = animationTime - start.mTime;
    float factor = std::abs(static_cast<float>(delta / keysDelta));

    return glm::toGlm((start.mValue - end.mValue) * factor + start.mValue);
}

glm::quat Animation::getInterpolatedRotation(std::string &nodeName, float animationTime) {
    if (rotationIndexes.count(nodeName) == 0) {
        return glm::quat(-1, -1, -1, -1);
    }

    if (rotationIndexes[nodeName].size() == 1) {
        return glm::toGlm(rotations[nodeName].begin()->mValue);
    }

    auto indexes = rotationIndexes[nodeName][static_cast<size_t>(animationTime)];
    auto start = rotations[nodeName][indexes.first];
    auto end = rotations[nodeName][indexes.second];

    auto keysDelta = end.mTime - start.mTime;
    auto delta = animationTime - start.mTime;
    float factor = std::abs(static_cast<float>(delta / keysDelta));

    aiQuaternion result;
    aiQuaternion.Interpolate(result, start.mValue, end.mValue, factor);

    return glm::toGlm(result);
}

std::string &Animation::getName() {
    return name;
}
