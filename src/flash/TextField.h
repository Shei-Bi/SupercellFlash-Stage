#pragma once
#include <flash/DisplayObject.h>

class TextField :public DisplayObject
{
private:
    /* data */
public:
    bool render(Matrix2x3*, ColorTransform* c, int, float);
};