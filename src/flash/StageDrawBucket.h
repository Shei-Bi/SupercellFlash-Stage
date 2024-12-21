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
    std::vector <float> vertices;
    std::vector<unsigned int> indices;
    int triangleCount;
    int pointCount;
    GLImage* texture;
    int renderConfig;

    // void StageDrawBucket() {
    //     ;
    // }
    void initForUse(GLImage* g, int rc) {
        texture = g;
        renderConfig = rc;
    }
    void reset() {
        vertices.clear();
        indices.clear();
        triangleCount = 0;
        pointCount = 0;
    }
};
