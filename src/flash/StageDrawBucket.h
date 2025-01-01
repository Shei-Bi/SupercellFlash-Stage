#pragma once
#include "SWFTexture.h"
#include <glad/glad.h>
#include <vector>
#include "GLImage.h"
class StageDrawBucket
{
private:
    /* data */
public:
    int indicesIndex;
    int triangleCount;
    int pointCount;
    GLImage* texture;
    int renderConfig;

    // void StageDrawBucket() {
    //     ;
    // }
    void initForUse(GLImage* g, int rc, int indicesIndex) {
        texture = g;
        renderConfig = rc;
        this->indicesIndex = indicesIndex;
        triangleCount = 0;
        pointCount = 0;
    }
    void reset() {
        indicesIndex = 0;
        triangleCount = 0;
        pointCount = 0;
    }
};
