#include "Animation.h"

Animation::Animation() {
}

void Animation::initializeBones(SCW::File* scwFile) {
    for (auto& node : scwFile->Nodes) {
        // if (node.Frames.size() != 1 || node.Name == "GEO") continue;
        std::string name = std::string(node.Name.data());
        boneTransforms[name] = &node;
    }
}