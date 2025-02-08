#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class DynamicMesh {
public:
    unsigned int VAO, VBO;
    unsigned char* buffer;
    int* boneIndices;
    std::vector<glm::mat4> boneInverseBindTransforms;
    DynamicMesh();
    ~DynamicMesh();
};