#pragma once
#include "SupercellSWF.h"
#include <flash/Stage.h>
#include <stdio.h>
#include <flash/StageSprite.h>
#include <Renderer.h>
#include <learnopengl_s.h>
#include <glm/glm.hpp>
#include <ResourceManager.h>

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
    //     shader = new Shader(
    //         R"(#ifdef GL_ES
    // precision highp float;
    // #else
    // #define highp 
    // #define mediump 
    // #define lowp 
    // #endif
    // attribute vec2 a_position;
    // attribute highp vec2 a_texCoord;
    // attribute lowp vec4 a_color;
    // attribute lowp vec3 a_color_2;
    // uniform vec4 myPMVMatrix;
    // varying highp vec2 v_texCoord;
    // varying lowp vec4 v_color;
    // varying lowp vec3 v_color_2;
    // void main(void)
    // {
    // const vec4 constantList = vec4(-1.0, 1.0, 0.0, 0.0);
    // gl_Position = myPMVMatrix * vec4(a_position,0.0,1.0) + constantList;
    // v_texCoord = a_texCoord;
    // // v_color = (a_color * constantList.yyyz + constantList.zzzy) * a_color.a;
    // // v_color=a_color;
    // v_color=vec4(1.0-a_color.r,1.0-a_color.g,1.0-a_color.b,1.0-a_color.a);
    // v_color_2 = a_color_2 * (1.0-a_color.a);
    // })",
    // R"(#ifdef GL_ES
    // precision lowp float;
    // #else
    // #define highp 
    // #define mediump 
    // #define lowp 
    // #endif
    // varying highp vec2 v_texCoord;
    // varying lowp vec4 v_color;
    // varying lowp vec3 v_color_2;
    // uniform lowp sampler2D s_texture;
    // void main (void)
    // {
    // vec4 tx = texture2D(s_texture, v_texCoord);
    // // gl_FragColor = tx * v_color;
    // gl_FragColor = tx * v_color + vec4(v_color_2,1.0) * tx.a;
    // // gl_FragColor=tx;
    // })");
    shader = new Shader(
        R"(#version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 2) in vec4 aColorMul;
    layout (location = 3) in vec3 aColorAdd;
    uniform vec4 myPMVMatrix;

    out vec2 texCoord;
    out vec4 colorMul;
    out vec3 colorAdd;

    void main()
    {
        const vec4 constantList = vec4(-1.0, 1.0, 0.0, 0.0);
        gl_Position = myPMVMatrix * vec4(aPos, 0.0, 1.0)+constantList;
        texCoord = aTexCoord;
        colorMul = aColorMul;
        colorAdd = aColorAdd;
    })",
        R"(
        #version 330 core
        // out vec4 FragColor;

    in vec2 texCoord;
    in vec4 colorMul;
    in vec3 colorAdd;

    // texture samplers
    uniform sampler2D TEX_SAMPLER;

    void main()
    {
        vec4 sample = texture2D(TEX_SAMPLER, texCoord);
        vec4 color = sample * colorMul;
        color.rgb += colorAdd * color.a;
        gl_FragColor = vec4(color.rgb * colorMul.a, color.a);
        // FragColor = sample;
    })");
    // R"(#version 330 core
    // out vec4 FragColor;
    // uniform sampler2D texture1;

    // in vec2 texCoord;
    // in vec4 colorMul;
    // in vec3 colorAdd;

    // void main()
    // {
    // FragColor = texture(texture1, texCoord);
    // return;
    //     // vec4 sample = texture2D(texture1, texCoord);
    //     // vec4 color = sample * colorMul;
    //     // color.rgb += colorAdd * color.a;
    //     // gl_FragColor = vec4(color.rgb * colorMul.a, color.a);
    //     // gl_FragColor = sample;
    // })");
}
void Stage::firstTimeShaderInit(Shader* shader, glm::mat4 mat4) {
    shader->use();
    shader->setUniformVector4("myPMVMatrix", mat4[0][0], mat4[1][1], 1.0, 1.0);
    glUniform1i(glGetUniformLocation(shader->ID, "TEX_SAMPLER"), 0);
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
    isCalculatingBounds = false;
    currentBounds = nullptr;
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}
void Stage::setBackgroundColor(int col32) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void updateBound(Rect* r, float x, float y) {
    if (r->left > x) r->left = x;
    else if (r->right < x) r->right = x;
    if (r->top > y) r->top = y;
    else if (r->bottom < y) r->bottom = y;
}
bool Stage::shapeStart(float left, float top, float right, float bottom, GLImage* texture, int renderConfig) {
    if (isCalculatingBounds) {
        if (currentBounds) {
            updateBound(currentBounds, left, top);
            updateBound(currentBounds, right, bottom);
        }
        return false;
    }

    // if (currentBucket->texture == texture) return true;
    for (int i = 0;i < bucketsUsed;i++) {
        if (buckets[i]->texture == texture && buckets[i]->renderConfig == renderConfig && buckets[i]->vertices.size() < 64000) {
            currentBucket = buckets[i];
            return true;
        }
    }
    if (bucketsUsed == bucketCapacity) increaseBucketCapacity(bucketsUsed * 5 / 4);
    currentBucket = buckets[bucketsUsed++];
    currentBucket->initForUse(texture, renderConfig);
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
    delete[] indices;
}

void Stage::render(float deltaTime, bool clear) {
    if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    resetRenderVariables();
    Matrix2x3* matrix = new Matrix2x3();
    matrix->a = pointSize;
    matrix->d = pointSize;
    ColorTransform* cT = new ColorTransform();
    StageSprit->render(matrix, cT, 0, deltaTime);
    delete matrix;
    delete cT;
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
        bindBlendMode(currentBucket->renderConfig & 0x380);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, currentBucket->triangleCount * 3, GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    resetRenderVariables();
}
bool Stage::bindBlendMode(int b) {
    int v2 = b & 0x380;
    switch ((v2 - 128) >> 7)
    {
    case 0:
        // glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    default:
        // glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;
    }
    return true;
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
    float s = width * 0.00097656;
    if (height / s < 576)
        s = height * 0.0017361;
    setPointSize(s);
}
void Stage::setPointSize(float s) {
    pointSize = s;
    updateStageSizeVariables();
}
void Stage::updateStageSizeVariables() {
    matrixX = (right - left) / pointSize;
    matrixY = (bottom - top) / pointSize;
}
void Stage::calculateDisplayObjectBounds(DisplayObject* a2, const Sprite* a3, Rect* a4) {
    isCalculatingBounds = true;
    currentBounds = a4;

    a2->render(&StageSprit->Matrix, &a2->colorTransform, 0, 0.0f);

    isCalculatingBounds = false;
    currentBounds = nullptr;
}
void Stage::updateBound(Rect* r, float x, float y) {
    if (r->left > x) r->left = x;
    else if (r->right < x) r->right = x;
    if (r->top > y) r->top = y;
    else if (r->bottom < y) r->bottom = y;
}


// shader->use();
// ResourceManager::getSupercellSWF("sc/background_vp.sc", "bgr_vp")->textures[0].GLImag->bind();
// unsigned int VBO, VAO, EBO;

// glGenVertexArrays(1, &VAO);
// glGenBuffers(1, &VBO);
// glGenBuffers(1, &EBO);

// glBindVertexArray(VAO);
// float vertices[] = {
//     //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//          800.0f, 600.0f, 1.0f, 1.0f, 1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,  // 右上
//          800.0f, 0.0f,1.0f, 0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,   // 右下
//         0.0f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,  // 左下
//         0.0f, 600.0f,0.0f, 1.0f, 1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f  // 左上
// };
// unsigned int indices[] = {
//     // 注意索引从0开始! 
//     // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
//     // 这样可以由下标代表顶点组合成矩形

//     0, 1, 2, // 第一个三角形
//     0,2,3  // 第二个三角形
// };
// // for (int i = 0;i < currentBucket->vertices.size();i++) {
// //     printf("vertices[%i]: %f\n", i, currentBucket->vertices[i]);
// // }
// // for (int i = 0;i < currentBucket->indices.size();i++) {
// //     printf("indices[%i]: %i\n", i, currentBucket->indices[i]);
// // }
// glBindBuffer(GL_ARRAY_BUFFER, VBO);
// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

// // position attribute
// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
// glEnableVertexAttribArray(0);
// // texture coord attribute
// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(2 * sizeof(float)));
// glEnableVertexAttribArray(1);

// glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(4 * sizeof(float)));
// glEnableVertexAttribArray(2);

// glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
// glEnableVertexAttribArray(3);

// glBindVertexArray(VAO);
// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// glDeleteVertexArrays(1, &VAO);
// glDeleteBuffers(1, &VBO);
// glDeleteBuffers(1, &EBO);
// return;