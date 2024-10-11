#pragma once
#include <flash/StageDrawBucket.h>
#include <flash/StageSprite.h>
#include <flash/DisplayObject.h>

class Stage
{
private:
    /* data */
public:
    Stage();
    static Stage* sm_pInstance;
    static Stage* getInstance();
    StageSprite* StageSprit;
    StageDrawBucket* currentBucket;
    static void constructInstance();
    void render(float deltaTime, bool clear);
    void renderBuckets();
    void resetRenderVariables();
    bool shapeStart(GLImage* texture);
    void addTriangles(int count);
    void addChild(DisplayObject* child);
};
