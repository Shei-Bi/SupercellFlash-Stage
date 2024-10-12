#pragma once
#include <flash/objects/SupercellSWF.h>
#include <flash/Stage.h>
#include <stdio.h>
#include <flash/StageSprite.h>
#include <Renderer.h>
#include <learnopengl_s.h>
#include <glm/glm.hpp>
#include <flash/transform/Matrix2x3.h>

Stage* Stage::sm_pInstance = nullptr;
Stage* Stage::getInstance() {
    return sm_pInstance;
}
void Stage::constructInstance()
{
    if (!Stage::sm_pInstance)
        Stage::sm_pInstance = new Stage();
}
void Stage::loadDefaultShader(int index) {
    shader = new Shader("C:/Users/EDY/source/repos/SupercellFlash-Stage/build/tools/test-tool/Debug/4.2.texture.vs", "C:/Users/EDY/source/repos/SupercellFlash-Stage/build/tools/test-tool/Debug/4.2.texture.fs");
}
void Stage::firstTimeShaderInit(Shader* shader, glm::mat4 mat4) {
    shader->use();
    shader->setUniformVector4("myPMVMatrix", mat4[0][0], mat4[1][1], 1.0, 1.0);
    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
}
Stage::Stage() {
    StageSprit = new StageSprite(10);
    currentBucket = new StageDrawBucket();
    top = 0;
    left = 0;
    right = 800;
    bottom = 600;
    loadDefaultShader(0);
}
bool Stage::shapeStart(GLImage* texture) {
    currentBucket->initForUse(texture);
    return true;
}
void Stage::addTriangles(int count) {
    int required = currentBucket->triangleCount * 3 + count * 3;
    // printf("Stage::addTriangles capacity %i, required %i\n", currentBucket->indices.capacity(), required);
    if (currentBucket->indices.capacity() < required) currentBucket->indices.reserve(currentBucket->indices.capacity() + 512 * 3);
    unsigned int* indices = new unsigned int[count * 3];
    for (int i = 0; i < count; i++) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }
    for (int i = 0;i < count * 3;i++) {
        // printf("%i", currentBucket->triangleCount * 3 + i);
        currentBucket->indices.emplace(currentBucket->indices.begin() + currentBucket->triangleCount * 3 + i, indices[i] + currentBucket->pointCount);
    }
    currentBucket->triangleCount += count;
    currentBucket->pointCount += count + 2;
}

void Stage::render(float deltaTime, bool clear) {
    if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    resetRenderVariables();
    Matrix2x3* mat = new Matrix2x3();
    StageSprit->render(mat);
    renderBuckets();
}
void Stage::renderBuckets() {
    shader->use();
    if (currentBucket->texture) currentBucket->texture->bind();
    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    for (int i = 0;i < currentBucket->vertices.size();i++) {
        printf("vertices[%i]: %f\n", i, currentBucket->vertices[i]);
    }
    for (int i = 0;i < currentBucket->indices.size();i++) {
        printf("indices[%i]: %i\n", i, currentBucket->indices[i]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, currentBucket->pointCount * 4 * sizeof(float), currentBucket->vertices.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentBucket->triangleCount * 3 * sizeof(unsigned int), currentBucket->indices.data(), GL_STREAM_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, currentBucket->triangleCount * 3, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    resetRenderVariables();
}
void Stage::resetRenderVariables() {
    currentBucket->reset();
}
void Stage::addChild(DisplayObject* child) {
    StageSprit->addChild(child);
}
void Stage::init(int top, int left, int width, int height) {
    this->top = top;
    this->left = left;
    this->right = left + width;
    this->bottom = top + height;
    Renderer::setViewport(this->top, this->left, this->right, this->bottom);
    Stage::firstTimeShaderInit(shader, Renderer::getPixelMatrix());
}