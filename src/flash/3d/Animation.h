#pragma once
#include "scw/File.hpp"

class Animation {
public:
    std::map<std::string, SCW::Node* > boneTransforms;
    Animation();
    void initializeBones(SCW::File* scwFile);
};