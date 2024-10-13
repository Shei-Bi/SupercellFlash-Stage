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
void Stage::increaseBucketCapacity(int c) {
    if (bucketCapacity < c) {
        StageDrawBucket** newArray = new StageDrawBucket * [c];
        for (int i = 0;i < bucketCapacity;i++) {
            newArray[i] = buckets[i];
        }
        for (int i = bucketCapacity;i < c;i++) newArray[i] = new StageDrawBucket();
        buckets = newArray;
        bucketCapacity = c;
    }
}
Stage::Stage() {
    buckets = nullptr;
    bucketCapacity = 0;
    bucketsUsed = 0;
    increaseBucketCapacity(150);
    StageSprit = new StageSprite(10);
    top = 0;
    left = 0;
    right = 800;
    bottom = 600;
    loadDefaultShader(0);
    abort = false;
}
bool Stage::shapeStart(GLImage* texture) {
    if (abort) return false;
    // if (currentBucket->texture == texture) return true;
    for (int i = 0;i < bucketsUsed;i++) {
        if (buckets[i]->texture == texture) {
            currentBucket = buckets[i];
            return true;
        }
    }
    if (bucketsUsed == bucketCapacity) increaseBucketCapacity(bucketsUsed * 5 / 4);
    currentBucket = buckets[bucketsUsed++];
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
    StageSprit->render(new Matrix2x3(), new ColorTransform(), deltaTime);
    renderBuckets();
}
void Stage::renderBuckets() {
    shader->use();
    for (int i = 0;i < bucketsUsed;i++) {
        StageDrawBucket* currentBucket = buckets[i];
        if (currentBucket->texture) currentBucket->texture->bind();
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
        glBufferData(GL_ARRAY_BUFFER, currentBucket->pointCount * 11 * sizeof(float), currentBucket->vertices.data(), GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentBucket->triangleCount * 3 * sizeof(unsigned int), currentBucket->indices.data(), GL_STREAM_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, currentBucket->triangleCount * 3, GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    resetRenderVariables();
}
void Stage::resetRenderVariables() {
    for (int i = 0;i < bucketsUsed;i++) buckets[i]->reset();
    bucketsUsed = 0;
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
// Stage::getInstance()->shader->use();
// 		((SupercellSWF*)ResourceManager::Resources[0].second)->textures[2].GLImag->bind();
// 		unsigned int VBO, VAO, EBO;

// 		glGenVertexArrays(1, &VAO);
// 		glGenBuffers(1, &VBO);
// 		glGenBuffers(1, &EBO);

// 		glBindVertexArray(VAO);
// 		float vertices[] = {
// 			//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
// 				 800.0f, 600.0f, 1.0f, 1.0f,   // 右上
// 				 800.0f, 0.0f,1.0f, 0.0f,   // 右下
// 				0.0f, 0.0f, 0.0f, 0.0f,   // 左下
// 				0.0f, 600.0f,0.0f, 1.0f    // 左上
// 		};
// 		unsigned int indices[] = {
// 			// 注意索引从0开始! 
// 			// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
// 			// 这样可以由下标代表顶点组合成矩形

// 			0, 1, 3, // 第一个三角形
// 			1, 2, 3  // 第二个三角形
// 		};
// 		// for (int i = 0;i < currentBucket->vertices.size();i++) {
// 		//     printf("vertices[%i]: %f\n", i, currentBucket->vertices[i]);
// 		// }
// 		// for (int i = 0;i < currentBucket->indices.size();i++) {
// 		//     printf("indices[%i]: %i\n", i, currentBucket->indices[i]);
// 		// }
// 		glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// 		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

// 		// position attribute
// 		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
// 		glEnableVertexAttribArray(0);
// 		// color attribute
// 		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 		// glEnableVertexAttribArray(1);
// 		// texture coord attribute
// 		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
// 		glEnableVertexAttribArray(1);

// 		glBindVertexArray(VAO);
// 		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// 		glDeleteVertexArrays(1, &VAO);
// 		glDeleteBuffers(1, &VBO);
// 		glDeleteBuffers(1, &EBO);