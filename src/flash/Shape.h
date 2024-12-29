#pragma once
#include <flash/DisplayObject.h>
class ShapeOriginal;
#include <vector>
#include "ShapeDrawBitmapCommand.h"
// #include <glad/glad.h>

class Shape :public DisplayObject
{
public:
    ShapeDrawBitmapCommand* commands;
    unsigned short commandSize;
public:
    static Shape* createShape(ShapeOriginal* original);
    bool render(Matrix2x3* mat, ColorTransform* c, int, float);
    bool collisionRender(Matrix2x3* mat);
};