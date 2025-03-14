#pragma once
#include <flash/DisplayObject.h>
#include "IInputListener.h"
#include <vector>

class MovieClip;

class Sprite :public DisplayObject, public IInputListener
{
private:
    /* data */
public:
    bool interactable;
    DisplayObject** children;
    short capacity;
    short size;
    Rect* hitArea;

    Sprite(/* args */);
    ~Sprite();
    Sprite(short initialCapacity);
    void allocateMemory(short capacity);
    void addChild(DisplayObject* child);
    void addChildAt(DisplayObject* child, short);
    virtual void removeChildAt(short);
    void removeChild(DisplayObject* child);
    void removeAllChildren();
    bool render(Matrix2x3*, ColorTransform* c, int, float);
    int getChildIndex(DisplayObject*);
    bool collisionRender(Matrix2x3*);
    void setInteractiveRecursive(bool);
    void setHitArea(Rect* hitArea);
    bool hitAreaTest(Matrix2x3* mat);

    void initScreenContainers(const char*, std::vector<MovieClip*>&);
    MovieClip* createScreenContainer(const char*, int);
    void addScreenContainer(const char*, int, std::vector<MovieClip*>&);

    int whyUninteractable();
};