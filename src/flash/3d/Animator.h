#pragma once
#include "Animation.h"
#include "Skeleton.h"

class Animator {
    int defaultAnimation;
    int currentAnimation;
public:
    float time;
    std::vector<Animation*> animations;
    Animator();
    void update(Skeleton* skeleton, float deltaTime);
    void updateAnimations(float deltaTime);
    void updateSkeleton(Skeleton* skeleton);
    void applyAnimation(Animation* currentAnimation, Skeleton::Bone* bone, const glm::mat4& parentTransform);
    void addAnimation(Animation* animation);
    void setDefaultAnimation(int index);
    int getDefaultAnimation();
    Animation* getCurrentAnimation();
    Animation* getAnimation(int index);
    void changeAnimationTo(int index);
};