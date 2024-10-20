#pragma once
#include <flash/objects/SWFTexture.h>
#include <glad/glad.h>
#include <flash/types/SWFContainer.hpp>

class StageDrawBucket
{
private:
    /* data */
public:
    sc::flash::SWFVector<float> vertices;
    sc::flash::SWFVector<unsigned int> indices;
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
