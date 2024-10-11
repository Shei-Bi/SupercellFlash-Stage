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
    Sprite(int initialCapacity);
    void allocateMemory(int capacity);
    void addChild(DisplayObject* child);
    bool render();
};