#pragma once
#include <glad/glad.h>
// #include <GLImage.h>
#include <flash/types/SWFContainer.hpp>

class GLImage;
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

    // void StageDrawBucket() {
    //     ;
    // }
    void initForUse(GLImage* g) {
        texture = g;
    }
    void reset() {
        vertices.clear();
        // indices.clear();
        triangleCount = 0;
        pointCount = 0;
    }
};
