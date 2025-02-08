#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
class Skeleton;
#include "DynamicMesh.h"
#include "scw/Geometry.hpp"

class Mesh {
public:
    unsigned int VAO, VBO, EBO;
    int indicesType;
    int indicesSize;
    int indicesOffset;
    int verticesSize;
    unsigned char* buffer;
    glm::mat4 matrix;
    Material* material;
    Mesh();
    ~Mesh();
    void render(const glm::mat4& transform, DynamicMesh* dynamicMesh);
    // void setVertices(int )
    DynamicMesh* createDynamicMesh(Skeleton* skeleton, SCW::Geometry* geometry);
};