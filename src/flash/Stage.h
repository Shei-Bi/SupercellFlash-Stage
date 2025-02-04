#pragma once
#include <flash/StageDrawBucket.h>
#include <flash/StageSprite.h>
#include <flash/DisplayObject.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include "IInputListener.h"

class Stage :public IInputListener
{
private:
    /* data */
public:
    Stage();
    ~Stage();
    static Stage* sm_pInstance;
    static Stage* getInstance();
    StageSprite* StageSprit;
    std::vector<unsigned int> indicesBucket;
    float* verticesBucket;
    int verticesBucketCapacity;
    int verticesBucketSize;
    // std::vector<float> verticesBucket;
    int triangleCount;
    int pointCount;
    //IndicesBucket 252
    //IndicesBucketCapacity 260
    //IndicesBucketSize 256
    //VertexBucket 264
    //VertexBucketCapacity 268
    //VertexBucketSize 272
    StageDrawBucket* currentBucket;//276
    StageDrawBucket** buckets;
    int bucketCapacity;
    int bucketsUsed;
    static void constructInstance();
    void render(float deltaTime, bool clear);
    void renderBuckets();
    void resetRenderVariables();
    bool shapeStart(float, float, float, float, GLImage* texture, int renderConfig, bool*);
    void start3D();
    void end3D();
    void addTriangles(int count);
    void addChild(DisplayObject* child);
    int left;
    int top;
    int right;
    int bottom;
    float pointSize;
    void init(int, int, int, int);
    void firstTimeShaderInit(Shader*, glm::mat4);
    void loadDefaultShader(int);
    Shader* shader;
    Shader* uber_shader;
    Shader* impostor_outline_shader;
    void increaseVertexBucketCapacity(int);
    void increaseBucketCapacity(int);
    bool abort;
    void setPointSize(float);
    void updateStageSizeVariables();
    float matrixX;
    float matrixY;
    bool bindBlendMode(int);
    void setBackgroundColor(int);
    bool isCalculatingBounds;
    Rect* currentBounds;
    float currentTouchX;
    float currentTouchY;
    void calculateDisplayObjectBounds(DisplayObject*, const Sprite*, Rect*);
    static void updateBound(Rect* r, float x, float y);
    std::vector<Sprite*>& getObjectsUnderPoint(float, float);
    std::vector<Sprite*> objectsUnderPoint;
    bool isRectVisibleInTouch(float, float, float, float);
    bool touchPressed(Touch&);
    bool touchMoved(Touch&);
    bool touchReleased(Touch&);

    unsigned int VBO, VAO, EBO;

    bool forceNewBucket;
};
