#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

class Mesh {
public:
    unsigned int VAO, VBO, EBO;
    int indicesType;
    int indicesSize;
    int indicesOffset;
    glm::mat4 matrix;
    Material* material;
    Mesh();
    ~Mesh();
    void render(const glm::mat4& transform);
    // void setVertices(int )
};