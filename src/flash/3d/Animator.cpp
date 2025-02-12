#include "Animator.h"

Animator::Animator() {
    time = 0.0f;
}

void Animator::update(Skeleton* skeleton, float deltaTime) {
    updateAnimations(deltaTime);
    updateSkeleton(skeleton);
}

void Animator::updateAnimations(float deltaTime) {
    time += deltaTime;
    auto data = getAnimation(currentAnimation)->animationData;
    float startTime = data->getStartFrame() / 30.0f;
    float endTime = data->getEndFrame() / 30.0f - data->getTransitionOutMs() / 1000.0f;
    if (time > endTime) {
        if (!data->getLooping()) {
            if (defaultAnimation >= 0) goto changeToDefault;
            time = endTime;
        }
        else {
            while (time > endTime)
                time -= (endTime - startTime);
        }
    }
    return;
changeToDefault:
    currentAnimation = defaultAnimation;
    time = getAnimation(currentAnimation)->animationData->getStartFrame() / 30.0f;
}

void Animator::updateSkeleton(Skeleton* skeleton) {
    applyAnimation(getAnimation(currentAnimation), skeleton->rootBone, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Animator::applyAnimation(Animation* currentAnimation, Skeleton::Bone* bone, const glm::mat4& parentTransform) {
    auto node = currentAnimation->boneTransforms.find(bone->name);
    auto currentTransform = bone->localBindTransform;
    // if (bone->name == "R_upperLeg_s") {
    //     currentTransform = glm::translate(currentTransform, glm::vec3(0.0f, 1.0f, 0.0f));
    //     currentTransform = glm::rotate(currentTransform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // }
    if (node != currentAnimation->boneTransforms.end()) {
        auto& frames = (*node).second->Frames;
        int nextFrame = 0;
        while (frames[nextFrame].ID / 30.0f <= time && nextFrame < frames.size() - 1)
            nextFrame++;

        currentTransform = nextFrame == 0 ? getLocalTransform(frames[0]) : getLocalTransformLerp(frames[nextFrame - 1], frames[nextFrame], (time - frames[nextFrame - 1].ID / 30.0f) / (frames[nextFrame].ID / 30.0f - frames[nextFrame - 1].ID / 30.0f));
        // currentTransform = getLocalTransform(frame);

    }
    // currentTransform = glm::
    currentTransform = parentTransform * currentTransform;
    bone->animatedTransform = currentTransform;
    // auto finalPos = bone->animatedTransform * bone->inverseBindTransformFromSCW;
    /*
[0] =
{x=1.00000000 y=0.00000000 z=0.00000000 ...}
[1] =
{x=0.00000000 y=1.00000000 z=0.00000000 ...}
[2] =
{x=0.00000000 y=0.00000000 z=1.00000000 ...}
[3] =
{x=-0.0189699996 y=4.30052805 z=-0.100253001 ...}

[0] =
{x=0.913418651 y=0.139988482 z=-0.382183999 ...}
[1] =
{x=-0.223196194 y=0.957495213 z=-0.182714224 ...}
[2] =
{x=0.340359747 y=0.252198756 z=0.905839324 ...}
[3] =
{x=0.169940993 y=3.65809011 z=-0.330466986 ...}
    */
    // printf("%s x: %.6f y: %.6f z: %.6f\n", bone->name.c_str(), finalPos[3][0], finalPos[3][1], finalPos[3][2]);

    for (Skeleton::Bone* bone : bone->childrens) {
        applyAnimation(currentAnimation, bone, currentTransform);
    }
}

void Animator::addAnimation(Animation* animation) {
    animations.push_back(animation);
}

void Animator::setDefaultAnimation(int index) {
    defaultAnimation = index;
}

int Animator::getDefaultAnimation() {
    return defaultAnimation;
}

Animation* Animator::getCurrentAnimation() {
    return getAnimation(currentAnimation);
}

Animation* Animator::getAnimation(int index) {
    for (auto anim : animations) if (anim->index == index) return anim;
    abort();
    return nullptr;
}

void Animator::changeAnimationTo(int index) {
    currentAnimation = index;
}
