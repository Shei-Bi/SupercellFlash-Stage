#pragma once
#include "SupercellSWF.h"
#include <flash/Stage.h>
#include <stdio.h>
#include <flash/StageSprite.h>
#include <Renderer.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <ResourceManager.h>
#include "Stage.h"
#include <glm/gtc/matrix_transform.hpp>

Stage* Stage::sm_pInstance = nullptr;
Stage* Stage::getInstance() {
    return sm_pInstance;
}
void Stage::constructInstance()
{
    if (!Stage::sm_pInstance)
        Stage::sm_pInstance = new Stage();
}

void Stage::initShaders(const glm::mat4& mat4) {
    shader->use();
    shader->setVec4("myPMVMatrix", mat4[0][0], mat4[1][1], 1.0, 1.0);
    glUniform1i(glGetUniformLocation(shader->ID, "TEX_SAMPLER"), 0);
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
        colorMul = (aColorMul * constantList.yyyz + constantList.zzzy) * aColorMul.a;
        colorAdd = aColorAdd * aColorMul.a;
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
        gl_FragColor = sample * colorMul + vec4(colorAdd, 0.0) * sample.a;
    })");
    uber_shader = new Shader(
        R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv0;

uniform mat4 view_matrix;
uniform float a_texMul;

// uniform vec4 u_stencilScaleOffset;

out vec2 v_texCoord;
out vec3 v_normal;
out vec2 v_texCoordStencil;

void main(void)
{
	vec4 pos = view_matrix * vec4(aPos.x, aPos.y, aPos.z,1.0);

	v_texCoord.xy = a_uv0 * a_texMul;
	v_normal = normalize(vec3(view_matrix * vec4(a_normal, 0.0)));

    v_texCoordStencil = a_uv0 * vec2(2.0f, -2.0f) + vec2(0.0f, 2.0f);

	gl_Position = pos;
})", R"(#version 330 core
in vec2 v_texCoord;
in vec3 v_normal;
in vec2 v_texCoordStencil;

uniform sampler2D diffuseTex;
uniform sampler2D stencilTex;

void main (void)
{
	vec4 color = vec4(1.0);
	color = texture2D(diffuseTex, v_texCoord.xy);

    vec4 stencilColor = texture2D(stencilTex, v_texCoordStencil);
    color.rgb = color.rgb * (1.0 - stencilColor.a) + stencilColor.rgb;

	gl_FragColor = color;
}
)");
    impostor_outline_shader = new Shader(
        R"(#version 330 core
layout (location = 0) in vec2 a_pos;
out vec2 v_texCoord;

void main(void)
{
	gl_Position = vec4(a_pos, 0.0, 1.0);
    v_texCoord = (a_pos + vec2(1, 1)) / 2.0f;
}
)",
R"(#version 330 core
in vec2 v_texCoord;

uniform sampler2D diffuseTex;

uniform vec4 u_outlineColor;
uniform vec2 u_outlineScale;

void main (void)
{
	vec4 diffuseSample = texture2D(diffuseTex, v_texCoord);

	vec4 diffuseColor = diffuseSample;

	if(diffuseSample.a == 1.0)
	{
		gl_FragColor = diffuseColor;
	}
	else
	{
		float a = texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(0.923880, 0.382683)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(0.382683, 0.923880)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(-0.382683, 0.923880)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(-0.923880, 0.382683)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(-0.923880, -0.382683)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(-0.382683, -0.923880)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(0.382683, -0.923880)).a;
		a += texture2D(diffuseTex, v_texCoord + u_outlineScale * vec2(0.923880, -0.382683)).a;

		vec3 color = u_outlineColor.rgb * (1.0 - diffuseSample.a) + diffuseColor.rgb;
		float alpha = min(diffuseColor.a + u_outlineColor.a * min(a, 1.0), 1.0);
		gl_FragColor = vec4(color * alpha, alpha);
	}
}
)");
}
void Stage::firstTimeShaderInit(Shader* shader, const glm::mat4& mat4) {
    shader->use();
    shader->setVec4("myPMVMatrix", mat4[0][0], mat4[1][1], 1.0, 1.0);
    glUniform1i(glGetUniformLocation(shader->ID, "TEX_SAMPLER"), 0);
}
void Stage::increaseBucketCapacity(int c) {
    if (bucketCapacity < c) {
        StageDrawBucket** newArray = new StageDrawBucket * [c];
        for (int i = 0;i < bucketCapacity;i++) {
            newArray[i] = buckets[i];
        }
        for (int i = bucketCapacity;i < c;i++) newArray[i] = new StageDrawBucket();
        if (buckets) delete buckets;
        buckets = newArray;
        bucketCapacity = c;
    }
}
void Stage::increaseVertexBucketCapacity(int c) {
    if (verticesBucketCapacity < c) {
        float* newArray = new float[c];
        for (int i = 0;i < verticesBucketSize;i++) {
            newArray[i] = verticesBucket[i];
        }
        if (verticesBucket) delete verticesBucket;
        verticesBucket = newArray;
        verticesBucketCapacity = c;
    }
}
Stage::Stage() {
    verticesBucket = nullptr;
    verticesBucketCapacity = 0;
    verticesBucketSize = 0;
    currentBucket = nullptr;
    forceNewBucket = false;
    buckets = nullptr;
    bucketCapacity = 0;
    bucketsUsed = 0;
    increaseBucketCapacity(150);
    StageSprit = new StageSprite(10);
    StageSprit->interactable = true;
    increaseVertexBucketCapacity(32768);
    top = 0;
    left = 0;
    right = 800;
    bottom = 600;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    loadDefaultShader(0);
    isCalculatingBounds = false;
    currentBounds = nullptr;
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    currentBlendMode = 0;
}
Stage::~Stage() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
bool Stage::isRectVisibleInTouch(float left, float top, float right, float bottom) {
    return left <= currentTouchX && right >= currentTouchX && top <= currentTouchY && bottom >= currentTouchY;
}
bool Stage::shapeStart(float left, float top, float right, float bottom, GLImage* texture, int renderConfig, bool* touchResultOut) {
    if (isCalculatingBounds) {
        if (currentBounds) {
            updateBound(currentBounds, left, top);
            updateBound(currentBounds, right, bottom);
            *touchResultOut = false;
        }
        else {
            *touchResultOut = isRectVisibleInTouch(left, top, right, bottom);
        }
        return false;
    }
    *touchResultOut = true;
    // if (currentBucket->texture == texture) return true;
    // if (forceNewBucket) {
    //     forceNewBucket = false;
    //     goto newBucket;
    // }
    if (!forceNewBucket && currentBucket && currentBucket->texture == texture && currentBucket->renderConfig == renderConfig) return true;
    // for (int i = 0;i < bucketsUsed;i++) {
    //     if (buckets[i]->texture == texture && buckets[i]->renderConfig == renderConfig) {
    //         currentBucket = buckets[i];
    //         return true;
    //     }
    // }
newBucket:
    if (bucketsUsed == bucketCapacity) increaseBucketCapacity(bucketsUsed * 5 / 4);
    currentBucket = buckets[bucketsUsed++];
    currentBucket->initForUse(texture, renderConfig, indicesBucket.size());
    return true;
}

void Stage::start3D() {
    renderBuckets();
}

void Stage::addTriangles(int count) {
    int required = triangleCount * 3 + count * 3;
    // printf("Stage::addTriangles capacity %i, required %i\n", currentBucket->indices.capacity(), required);
    if (indicesBucket.capacity() < required) indicesBucket.reserve(indicesBucket.capacity() * 5 / 4);
    // if (currentBucket->indices.capacity() < required) currentBucket->indices.reserve(currentBucket->indices.capacity() + 512 * 3);
    // auto indices = indicesBucket.end();
    for (int i = 0; i < count; i++) {
        // indices[i * 3] = triangleCount;
        // indices[i * 3 + 1] = triangleCount + i + 1;
        // indices[i * 3 + 2] = triangleCount + i + 2;
        indicesBucket.push_back(pointCount);
        indicesBucket.push_back(pointCount + i + 1);
        indicesBucket.push_back(pointCount + i + 2);
    }
    if (verticesBucketSize + (2 + count) * 11 > verticesBucketCapacity)
        increaseVertexBucketCapacity(verticesBucketSize * 5 / 4 + (2 + count) * 11);
    triangleCount += count;
    pointCount += count + 2;
    currentBucket->triangleCount += count;
    currentBucket->pointCount += count + 2;
}

void Stage::render(float deltaTime, bool clear) {
    if (clear) {
        // glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearStencil(0);
        glStencilMask(0x00);
        // glDepthMask(GL_FALSE);
    }
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

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pointCount * 11 * sizeof(float), verticesBucket, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * 3 * sizeof(unsigned int), indicesBucket.data(), GL_STREAM_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    for (int i = 0;i < bucketsUsed;i++) {
        StageDrawBucket* currentBucket = buckets[i];
        if (currentBucket->stencilType != 0) {
            // switch (currentBucket->stencilType) {
            // case 2:
            //     printf("Stencil test started, stack size: %d\n", stencilStack.size());
            //     // if (stencilStack.size() > 0) {
            //     stencilStack.push_back(2);
            //     //     break;//multiple stencil not supported yet
            //     // }
            //     // stencilStack.push_back(2);
            //     // glEnable(GL_STENCIL_TEST);
            //     // glStencilFunc(GL_ALWAYS, 1, 0xFF); // ¡ì?¡ì?¡ì?¡ì?¡ì¨ª¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì? ¡ì¨¤¡ì?¡ì?¡ì¨¤¡ì?¡ì?¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì¨ª¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?
            //     // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            //     // glStencilMask(0xFF); // ¡ì?¡ì?¡ì?¡ì?¡ì¨¦¡ì?¡ì?¡ì? ¡ì?¡ì?¡ì¨¢¡ì?¡ì?¡ì? ¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì¨ª¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?
            //     // glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            //     // glDepthMask(GL_FALSE);
            //     // glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
            //     break;
            // case 3:
            //     // printf("Stencil test ended\n");
            //     // if (stencilStack.size() > 0) {
            //     //     break;//multiple stencil not supported yet
            //     // }
            //     // glStencilFunc(GL_EQUAL, 1, 0xFF);
            //     // glStencilMask(0x00); // ¡ì¨¤¡ì?¡ì?¡ì?¡ì?¡ì¨¦¡ì?¡ì?¡ì? ¡ì?¡ì?¡ì¨¢¡ì?¡ì?¡ì? ¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì?¡ì¨ª¡ì? ¡ì?¡ì?¡ì?¡ì?¡ì?
            //     // glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            //     break;
            // case 4:
            //     printf("Stencil ended, stack size: %d\n", stencilStack.size());
            //     if (stencilStack.size() <= 0) printf("Stencil stack size == 0 !!!\n");
            //     if (stencilStack.size() > 0)
            //         stencilStack.pop_back();
            //     // if (stencilStack.size() > 0) {
            //     //     break;//multiple stencil not supported yet
            //     // }
            //     // glDisable(GL_STENCIL_TEST);
            //     break;
            // }
        }
        else {
            if (currentBucket->texture) currentBucket->texture->bind();
            bindBlendMode(currentBucket->renderConfig & 0x380);
            glDrawElements(GL_TRIANGLES, currentBucket->triangleCount * 3, GL_UNSIGNED_INT, (const void*)(currentBucket->indicesIndex * sizeof(unsigned int)));
        }

    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    resetRenderVariables();
}
bool Stage::bindBlendMode(int b) {
    int v2 = b & 0x380;
    if (currentBlendMode == v2) return false;
    currentBlendMode = v2;
    // printf("blend mode %d\n", v2);
    switch ((v2 - 128) >> 7) {
    case -1:
        // glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case 0:
        // glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    default:
        ;
        // abort();
    }
    return true;
}
void Stage::resetRenderVariables() {
    currentBucket = nullptr;
    for (int i = 0;i < bucketsUsed;i++) {
        // delete buckets[i];
    }
    bucketsUsed = 0;
    verticesBucketSize = 0;
    indicesBucket.clear();
    triangleCount = 0;
    pointCount = 0;
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
bool Stage::touchPressed(Touch& touch) {
    auto scaledTouch = touch;
    scaledTouch.x /= pointSize;
    scaledTouch.y /= pointSize;
    scaledTouch.previousX /= pointSize;
    scaledTouch.previousY /= pointSize;
    scaledTouch.initialX /= pointSize;
    scaledTouch.initialY /= pointSize;
    touchContainer = getObjectsUnderPoint(scaledTouch.x, scaledTouch.y);
    for (auto s = touchContainer.rbegin();s != touchContainer.rend();s++) {
        if ((*s)->touchPressed(scaledTouch)) break;
    }
#ifdef MOVIECLIP_DEBUG
    printf("touchin result:\n----------------------\n");
    for (auto s = touchContainer.rbegin();s != touchContainer.rend();s++) {
        if ((*s)->name != nullptr) printf("%s\n", (*s)->name);
    }
    printf("----------------------\n");
#endif
    return true;
}
bool Stage::touchMoved(Touch& touch) {
    auto scaledTouch = touch;
    scaledTouch.x /= pointSize;
    scaledTouch.y /= pointSize;
    scaledTouch.previousX /= pointSize;
    scaledTouch.previousY /= pointSize;
    scaledTouch.initialX /= pointSize;
    scaledTouch.initialY /= pointSize;
    for (auto s = touchContainer.rbegin();s != touchContainer.rend();s++) {
        if ((*s)->touchMoved(scaledTouch)) break;
    }
    return true;
}
bool Stage::touchReleased(Touch& touch) {
    auto scaledTouch = touch;
    scaledTouch.x /= pointSize;
    scaledTouch.y /= pointSize;
    scaledTouch.previousX /= pointSize;
    scaledTouch.previousY /= pointSize;
    scaledTouch.initialX /= pointSize;
    scaledTouch.initialY /= pointSize;
    for (auto s = touchContainer.rbegin();s != touchContainer.rend();s++) {
        if ((*s)->touchReleased(scaledTouch)) break;
    }
    return true;
}
void Stage::setStencilRenderingState(int state) {
    if (bucketsUsed == bucketCapacity) increaseBucketCapacity(bucketsUsed * 5 / 4);
    currentBucket = buckets[bucketsUsed++];
    currentBucket->initForUse(nullptr, 0, indicesBucket.size());
    currentBucket->stencilType = state;
}
std::vector<Sprite*>& Stage::getObjectsUnderPoint(float x, float y) {
    isCalculatingBounds = true;
    currentTouchX = x;
    currentTouchY = y;
    objectsUnderPoint.resize(0);
    auto m = new Matrix2x3();
    StageSprit->collisionRender(m);
    currentTouchX = 0.0f;
    currentTouchY = 0.0f;
    isCalculatingBounds = false;
    delete m;
    return objectsUnderPoint;
}