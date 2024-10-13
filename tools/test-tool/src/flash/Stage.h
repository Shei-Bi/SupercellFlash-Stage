#pragma once
#include <flash/StageDrawBucket.h>
#include <flash/StageSprite.h>
#include <flash/DisplayObject.h>
#include <learnopengl_s.h>
#include <glm/glm.hpp>

using Matrix2x3 = sc::flash::Matrix2x3;

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
    StageDrawBucket** buckets;
    int bucketCapacity;
    int bucketsUsed;
    static void constructInstance();
    void render(float deltaTime, bool clear);
    void renderBuckets();
    void resetRenderVariables();
    bool shapeStart(GLImage* texture);
    void addTriangles(int count);
    void addChild(DisplayObject* child);
    int left;
    int top;
    int right;
    int bottom;
    void init(int, int, int, int);
    void firstTimeShaderInit(Shader*, glm::mat4);
    void loadDefaultShader(int);
    Shader* shader;
    void increaseBucketCapacity(int);
    bool abort;
};
