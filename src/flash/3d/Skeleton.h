#pragma once
#include "scw/File.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Animation.h"
#include "Mesh.h"

glm::mat4 getLocalTransform(SCW::KeyFrame& frame);
glm::mat4 getLocalTransformLerp(SCW::KeyFrame& frame, SCW::KeyFrame& frame2, float t);

class Skeleton {
public:
    struct Bone {
    public:
        const std::string name;
        std::vector<Bone*> childrens;
        glm::mat4 inverseBindTransform;
        glm::mat4 inverseBindTransformFromSCW;
        const glm::mat4 localBindTransform;
        glm::mat4 animatedTransform;
        unsigned char index;
        Bone(const std::string name, const glm::mat4 mat, unsigned char index);
    };
    std::vector<Bone> bones;
    Bone* rootBone;
    void initialize(SCW::File* scwFile);
    Bone* findBone(const std::string& name);
    void beginUpdateMesh(Mesh* mesh, DynamicMesh* dynamicMesh);
    void updateMesh(Mesh* mesh, DynamicMesh* dynamicMesh);
    void finishUpdateMesh(Mesh* mesh, DynamicMesh* dynamicMesh);
    // void animate();
    void printDebugInfo();
};