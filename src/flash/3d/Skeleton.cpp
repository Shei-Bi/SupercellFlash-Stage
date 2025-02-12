#include "Skeleton.h"

void calculateInverseBindTransform(Skeleton::Bone* bone, const glm::mat4& parentTransform) {
    glm::mat4 transform = bone->localBindTransform * parentTransform;
    bone->inverseBindTransform = glm::inverse(transform);
    for (auto bone : bone->childrens) {
        calculateInverseBindTransform(bone, transform);
    }
}
void Skeleton::initialize(SCW::File* scwFile) {
    bones.reserve(scwFile->Nodes.size());
    for (auto node : scwFile->Nodes) {
        if (node.Frames.size() != 1 || node.Name == "GEO") continue;
        std::string name = std::string(node.Name.data());
        bones.emplace_back(name, getLocalTransform(node.Frames[0]), (unsigned char)bones.size());
        if (node.ParentName.length() == 0) {
            rootBone = &bones.back();
        }
        else {
            std::string parentName = std::string(node.ParentName.data());
            findBone(parentName)->childrens.push_back(&bones.back());
        }
        // if (node.ParentName.length())
        //     printf("(%s) ", node.ParentName.data());
        // printf("%s\n", node.Name.data());
    }
    calculateInverseBindTransform(rootBone, glm::mat4(1.0f));
    // abort();
}

Skeleton::Bone* Skeleton::findBone(const std::string& name) {
    for (auto& bone : bones) {
        if (bone.name == name) return &bone;
    }
    abort();
}

void Skeleton::beginUpdateMesh(Mesh* mesh, DynamicMesh* dynamicMesh) {
    glBindBuffer(GL_ARRAY_BUFFER, dynamicMesh->VBO);
    dynamicMesh->buffer = (unsigned char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, mesh->verticesSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skeleton::updateMesh(Mesh* mesh, DynamicMesh* dynamicMesh) {
    for (int i = 0;i < mesh->verticesSize;i += 52) {
        glm::vec4 finalPos(0.0f);
        glm::vec4 pos(*(float*)(mesh->buffer + i + 0), *(float*)(mesh->buffer + i + 4), *(float*)(mesh->buffer + i + 8), 1.0f);
        for (int j = 0;j < 4;j++) {
            float weight = *(float*)(mesh->buffer + i + 36 + j * 4);
            if (weight == 0.0f) break;
            unsigned char e = *(mesh->buffer + i + 32 + j);
            int boneIndex = dynamicMesh->boneIndices[e];

            // auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            // auto mat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            // if (boneIndex > 5) mat = glm::translate(mat, glm::vec3(0.0f, 1.0f, 0.0f));
            /*dynamicMesh->boneInverseBindTransforms[e]*/
            // printf("%s\n", bones[boneIndex].name.c_str());
            auto mat = bones[boneIndex].animatedTransform * bones[boneIndex].inverseBindTransformFromSCW;
            finalPos += ((mat * pos) * weight);
        }
        *(float*)(dynamicMesh->buffer + i + 0) = finalPos.x;
        *(float*)(dynamicMesh->buffer + i + 4) = finalPos.y;
        *(float*)(dynamicMesh->buffer + i + 8) = finalPos.z;
        memcpy(dynamicMesh->buffer + i + 12, mesh->buffer + i + 12, 52 - 12);
    }
}

void Skeleton::finishUpdateMesh(Mesh* mesh, DynamicMesh* dynamicMesh) {
    glBindBuffer(GL_ARRAY_BUFFER, dynamicMesh->VBO);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skeleton::printDebugInfo() {
    for (auto& bone : bones) {
        auto finalPos = bone.animatedTransform * bone.inverseBindTransform;
        printf("%s x: %.3f y: %.3f z: %.3f\n", bone.name.c_str(), finalPos[3][0], finalPos[3][1], finalPos[3][2]);
    }
}

Skeleton::Bone::Bone(const std::string name, const glm::mat4 mat, unsigned char index) :name(name), localBindTransform(mat), index(index), inverseBindTransformFromSCW(1.0f) {
    ;
}

glm::mat4 createTRS(float translationX, float translationY, float translationZ, float quaternionX, float quaternionY, float quaternionZ, float quaternionW, float scaleX, float scaleY, float scaleZ) {
    float xs = quaternionX * 2.0f;
    float ys = quaternionY * 2.0f;
    float zs = quaternionZ * 2.0f;
    float wx = quaternionW * xs;
    float wy = quaternionW * ys;
    float wz = quaternionW * zs;
    float xx = quaternionX * xs;
    float xy = quaternionX * ys;
    float xz = quaternionX * zs;
    float yy = quaternionY * ys;
    float yz = quaternionY * zs;
    float zz = quaternionZ * zs;
    /* 0  4  8  12
       16 20 24 28
       32 36 40 44
       48 52 56 60
    */
    return glm::mat4(
        scaleX * (1.0f - (yy + zz)), scaleX * (xy + wz), scaleX * (xz - wy), 0.0f,
        scaleY * (xy - wz), scaleY * (1.0f - (xx + zz)), scaleY * (yz + wx), 0.0f,
        scaleZ * (xz + wy), scaleZ * (yz - wx), scaleZ * (1.0f - (xx + yy)), 0.0f,
        translationX, translationY, translationZ, 1.0f);
}
glm::mat4 getLocalTransform(SCW::KeyFrame& frame) {
    // auto mat = glm::translate(glm::mat4(1.0f), glm::vec3(frame.Translation.X, frame.Translation.Y, frame.Translation.Z)) * glm::mat4_cast(glm::quat(frame.Rotation.W, frame.Rotation.X, frame.Rotation.Y, frame.Rotation.Z)) * glm::scale(glm::mat4(1.0f), glm::vec3(frame.Scale.X, frame.Scale.Y, frame.Scale.Z));
    auto mat2 = createTRS(frame.Translation.X, frame.Translation.Y, frame.Translation.Z, frame.Rotation.X, frame.Rotation.Y, frame.Rotation.Z, frame.Rotation.W, frame.Scale.X, frame.Scale.Y, frame.Scale.Z);
    return mat2;
}

glm::mat4 getLocalTransformLerp(SCW::KeyFrame& frame, SCW::KeyFrame& frame2, float t) {
    auto translation = SCW::Vector3();
    auto rotation = SCW::Quaternion();
    auto scale = SCW::Vector3();
    translation.X = frame.Translation.X + (frame2.Translation.X - frame.Translation.X) * t;
    translation.Y = frame.Translation.Y + (frame2.Translation.Y - frame.Translation.Y) * t;
    translation.Z = frame.Translation.Z + (frame2.Translation.Z - frame.Translation.Z) * t;

    float dot = frame.Rotation.W * frame2.Rotation.W + frame.Rotation.X * frame2.Rotation.X + frame.Rotation.Y * frame2.Rotation.Y + frame.Rotation.Z * frame2.Rotation.Z;
    float tI = 1.0f - t;
    if (dot < 0.0f) {
        rotation.X = frame.Rotation.X * tI - frame2.Rotation.X * t;
        rotation.Y = frame.Rotation.Y * tI - frame2.Rotation.Y * t;
        rotation.Z = frame.Rotation.Z * tI - frame2.Rotation.Z * t;
        rotation.W = frame.Rotation.W * tI - frame2.Rotation.W * t;
    }
    else {
        rotation.X = frame.Rotation.X * tI + frame2.Rotation.X * t;
        rotation.Y = frame.Rotation.Y * tI + frame2.Rotation.Y * t;
        rotation.Z = frame.Rotation.Z * tI + frame2.Rotation.Z * t;
        rotation.W = frame.Rotation.W * tI + frame2.Rotation.W * t;
    }
    float dot2 = sqrtf(rotation.X * rotation.X + rotation.Y * rotation.Y + rotation.Z * rotation.Z + rotation.W * rotation.W);
    rotation.X /= dot2;
    rotation.Y /= dot2;
    rotation.Z /= dot2;
    rotation.W /= dot2;

    scale.X = frame.Scale.X + (frame2.Scale.X - frame.Scale.X) * t;
    scale.Y = frame.Scale.Y + (frame2.Scale.Y - frame.Scale.Y) * t;
    scale.Z = frame.Scale.Z + (frame2.Scale.Z - frame.Scale.Z) * t;

    return createTRS(translation.X, translation.Y, translation.Z, rotation.X, rotation.Y, rotation.Z, rotation.W, scale.X, scale.Y, scale.Z);
}
