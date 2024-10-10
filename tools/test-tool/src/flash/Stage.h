#pragma once
#include <glad/glad.h>
#include <flash/StageDrawBucket.h>

class Stage
{
private:
    /* data */
public:
    Stage();
    static Stage* sm_pInstance;
    static Stage* getInstance();
    StageDrawBucket* currentBucket;
    static void constructInstance();
    void render(float deltaTime, bool clear);
    bool shapeStart(GLImage* texture);
    void addTriangles(int count);
};
