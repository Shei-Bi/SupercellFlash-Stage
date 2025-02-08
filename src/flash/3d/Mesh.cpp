#include "Mesh.h"
#include <glad/glad.h>
#include "Skeleton.h"

Mesh::Mesh() {
    matrix = glm::mat4(1.0f);
    VAO = 0;
    VBO = 0;
    EBO = 0;
    indicesType = 0;
    indicesSize = 0;
    indicesOffset = 0;
    verticesSize = 0;
    buffer = nullptr;
}

Mesh::~Mesh() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
    }
    if (buffer) {
        delete[] buffer;
    }
}

void Mesh::render(const glm::mat4& transform, DynamicMesh* dynamicMesh) {
    material->bind(transform * matrix);

    glBindVertexArray(VAO);
    if (dynamicMesh) {
        glBindBuffer(GL_ARRAY_BUFFER, dynamicMesh->VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 52, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 52, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 52, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    glDrawElements(GL_TRIANGLES, indicesSize, indicesType, (void*)indicesOffset);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

DynamicMesh* Mesh::createDynamicMesh(Skeleton* skeleton, SCW::Geometry* geometry) {
    DynamicMesh* dynamicMesh = new DynamicMesh();
    glGenBuffers(1, &dynamicMesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, dynamicMesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    int* indices = new int[geometry->Skins.Joints.size()];
    for (int i = 0;i < geometry->Skins.Joints.size();i++) {
        auto bone = skeleton->findBone(std::string(geometry->Skins.Joints[i].data()));
        indices[i] = bone->index;
        auto& mat = geometry->Skins.InverseBindMatrices[i];
        // if (glm::mat4(1.0f) != bone->inverseBindTransformFromSCW && glm::mat4(mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3), mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3), mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3), mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3)) != bone->inverseBindTransformFromSCW)
        //     abort();
        if (glm::mat4(1.0f) == bone->inverseBindTransformFromSCW)
            bone->inverseBindTransformFromSCW = glm::mat4(
                mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3),
                mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3),
                mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3),
                mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
        dynamicMesh->boneInverseBindTransforms.emplace_back(
            mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3),
            mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3),
            mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3),
            mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
        // dynamicMesh->boneInverseBindTransforms.emplace_back(
        //     mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0),
        //     mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1),
        //     mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2),
        //     mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3));
    }
    // geometry->Skins.Joints
    // delete indices;
    dynamicMesh->boneIndices = indices;
    return dynamicMesh;
}

