#pragma once
#include <flash/DisplayObject.h>
#include "IInputListener.h"

class Sprite :public DisplayObject, public IInputListener
{
private:
    /* data */
public:
    bool interactable;
    DisplayObject** children;
    short capacity;
    short size;

    Sprite(/* args */);
    ~Sprite();
    Sprite(short initialCapacity);
    void allocateMemory(short capacity);
    void addChild(DisplayObject* child);
    void addChildAt(DisplayObject* child, short);
    virtual void removeChildAt(short);
    void removeChild(DisplayObject* child);
    bool render(Matrix2x3*, ColorTransform* c, int, float);
    int getChildIndex(DisplayObject*);
    bool collisionRender(Matrix2x3*);
    void setInteractiveRecursive(bool);
};