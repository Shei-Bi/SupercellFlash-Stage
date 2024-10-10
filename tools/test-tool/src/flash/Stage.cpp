#include <flash/Stage.h>
// #include <GLImage.h>
#include <stdio.h>

Stage* Stage::sm_pInstance = nullptr;
Stage* Stage::getInstance() {
    return sm_pInstance;
}
void Stage::constructInstance()
{
    if (!Stage::sm_pInstance)
        Stage::sm_pInstance = new Stage();
}
Stage::Stage() {
    currentBucket = new StageDrawBucket();
}
bool Stage::shapeStart(GLImage* texture) {
    currentBucket->initForUse(texture);
    return true;
}
void Stage::addTriangles(int count) {
    int required = currentBucket->triangleCount * 3 + count * 3;
    // printf("Stage::addTriangles capacity %i, required %i\n", currentBucket->indices.capacity(), required);
    if (currentBucket->indices.capacity() < required) currentBucket->indices.resize(required);
    unsigned int* indices = new unsigned int[count * 3];
    for (int i = 0; i < count; i++) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }
    for (int i = 0;i < count * 3;i++) {
        // printf("%i", currentBucket->triangleCount * 3 + i);
        currentBucket->indices[currentBucket->triangleCount * 3 + i] = indices[i] + currentBucket->pointCount;
    }
    currentBucket->triangleCount += count;
    currentBucket->pointCount += count + 2;
}
void Stage::render(float deltaTime, bool clear) {
    if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // currentBucket->texture->bind();
    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // for (int i = 0;i < currentBucket->vertices.size();i++) {
    //     printf("vertices[%i]: %f\n", i, currentBucket->vertices[i]);
    // }
    // for (int i = 0;i < currentBucket->indices.size();i++) {
    //     printf("indices[%i]: %i\n", i, currentBucket->indices[i]);
    // }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, currentBucket->pointCount * 4 * sizeof(float), currentBucket->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentBucket->triangleCount * 3 * sizeof(unsigned int), currentBucket->indices.data(), GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, currentBucket->triangleCount * 3, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    currentBucket->reset();
}