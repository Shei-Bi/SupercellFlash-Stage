#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh() {
    matrix = glm::mat4(1.0f);
    VAO = 0;
    VBO = 0;
    EBO = 0;
    indicesType = 0;
    indicesSize = 0;
    indicesOffset = 0;
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
}

void Mesh::render(const glm::mat4& transform) {
    material->bind();
    material->shader->setMat4("view_matrix", transform * matrix);
    material->shader->setFloat("a_texMul", 1.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, indicesType, (void*)indicesOffset);
    glBindVertexArray(0);
}
