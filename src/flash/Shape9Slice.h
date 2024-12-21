#pragma once
#include <flash/Shape.h>

class Shape9Slice :public Shape
{
public:
    Rect* scalingGrid;
    static Shape9Slice* createShape(ShapeOriginal* original, Rect* rect);
    bool render(Matrix2x3* mat, ColorTransform* c, int, float);
};