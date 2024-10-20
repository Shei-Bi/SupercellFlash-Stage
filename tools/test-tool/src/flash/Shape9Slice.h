#pragma once
#include <flash/Shape.h>

class Shape9Slice :public Shape
{
public:
    Rect* scalingGrid;
    static Shape9Slice* createShape(sc::flash::ShapeOriginal* original, Rect* rect);
    bool render(Matrix2x3* mat, ColorTransform* c, int, float);
};