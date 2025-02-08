#pragma once
#include "Animation.h"
#include "Skeleton.h"

class Animator {
public:
    float time;
    Animation* currentAnimation;
    Animator();
    void update(Skeleton* skeleton, float deltaTime);
    void updateSkeleton(Skeleton* skeleton);
    void applyAnimation(Skeleton::Bone* bone, const glm::mat4& parentTransform);
};