#include "DynamicMesh.h"
#include <glad/glad.h>

DynamicMesh::DynamicMesh() {
    VBO = 0;
    boneIndices = nullptr;
    buffer = nullptr;
}

DynamicMesh::~DynamicMesh() {
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (boneIndices != nullptr) delete[] boneIndices;
}