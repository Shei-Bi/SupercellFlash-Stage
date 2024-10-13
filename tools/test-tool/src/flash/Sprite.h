#pragma once
#include <flash/DisplayObject.h>

class Sprite :public DisplayObject
{
private:
    /* data */
public:
    DisplayObject** children;
    short capacity;
    short size;

    Sprite(/* args */);
    ~Sprite();
    Sprite(short initialCapacity);
    void allocateMemory(short capacity);
    void addChild(DisplayObject* child);
    void addChildAt(DisplayObject* child, short);
    void removeChildAt(short);
    bool render(Matrix2x3*, ColorTransform* c, float);
};