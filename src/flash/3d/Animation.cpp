#include "Animation.h"
#include "ResourceManager.h"

Animation::Animation() {
}

void Animation::valueChanged() {
    fileReference = ResourceManager::getSC3D(animationData->getFileName());
    initializeBones();
    setEndFrame(animationData->getEndFrame());
}

void Animation::initializeBones() {
    for (auto& node : fileReference->Nodes) {
        // if (node.Frames.size() != 1 || node.Name == "GEO") continue;
        std::string name = std::string(node.Name.data());
        boneTransforms[name] = &node;
    }
}

void Animation::setEndFrame(int index) {
    if (index < 0) endFrame = fileReference->Header.LastFrame;
    else endFrame = index;
}
