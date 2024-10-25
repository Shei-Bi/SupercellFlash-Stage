#pragma once
#include <flash/transform/Matrix2x3.h>
#include <flash/transform/ColorTransform.h>
#include <core/math/rect.h>

using Matrix2x3 = sc::flash::Matrix2x3;
using ColorTransform = sc::flash::ColorTransform;
using Rect = sc::RectF;

class DisplayObject
{
public:
    ColorTransform colorTransform;
    bool visible;
    Matrix2x3 Matrix;
    int renderConfig;
public:
    DisplayObject(/* args */);
    ~DisplayObject();
    void setX(float);
    void setY(float);
    float getX();
    float getY();
    void setXY(float, float);
    void setPixelSnappedXY(float, float);
    void setScale(float);
    void setScaleX(float);
    void setScaleY(float);
    virtual bool render(Matrix2x3*, ColorTransform*, int, float);
    void setBlendMode(int);
    void setAlpha(float);
};