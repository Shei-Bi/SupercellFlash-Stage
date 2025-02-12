#pragma once
#include "scw/File.hpp"
#include "data/LogicAnimationData.h"
#include "data/LogicFaceData.h"
#include "flash/MovieClip.h"

class Animation {
public:
    int index;
    int endFrame;
    SCW::File* fileReference;
    std::map<std::string, SCW::Node* > boneTransforms;
    LogicAnimationData* animationData;
    LogicFaceData* faceData;
    MovieClip* faceClip;
    Animation();
    void valueChanged();
    void initializeBones();
    void setEndFrame(int index);
};