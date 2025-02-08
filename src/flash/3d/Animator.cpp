#include "Animator.h"

Animator::Animator() {
    time = 0.0f;
}

void Animator::update(Skeleton* skeleton, float deltaTime) {
    time += deltaTime;
    while (time > 6.0f)
        time -= 6.0f;
    updateSkeleton(skeleton);
}

void Animator::updateSkeleton(Skeleton* skeleton) {
    applyAnimation(skeleton->rootBone, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Animator::applyAnimation(Skeleton::Bone* bone, const glm::mat4& parentTransform) {
    auto node = currentAnimation->boneTransforms.find(bone->name);
    auto currentTransform = bone->localBindTransform;
    // if (bone->name == "R_upperLeg_s") {
    //     currentTransform = glm::translate(currentTransform, glm::vec3(0.0f, 1.0f, 0.0f));
    //     currentTransform = glm::rotate(currentTransform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // }
    if (node != currentAnimation->boneTransforms.end()) {
        for (auto& frame : (*node).second->Frames) {
            // printf("%d\n", frame.ID);
            if (frame.ID / 30.0f > time) {
                currentTransform = getLocalTransform(frame);
                break;
            }
        }
        // currentTransform =
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
        applyAnimation(bone, currentTransform);
    }
}
