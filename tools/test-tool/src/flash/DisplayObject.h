#pragma once
#include <flash/transform/Matrix2x3.h>
#include <flash/transform/ColorTransform.h>

using Matrix2x3 = sc::flash::Matrix2x3;
using ColorTransform = sc::flash::ColorTransform;

class DisplayObject
{
public:
    ColorTransform colorTransform;
    Matrix2x3 Matrix;
public:
    DisplayObject(/* args */);
    ~DisplayObject();
    void setX(float);
    void setY(float);
    void setXY(float, float);
    void setScale(float);
    void setScaleX(float);
    void setScaleY(float);
    virtual bool render(Matrix2x3*, ColorTransform*, float);

};