#pragma once
#include <flash/transform/Matrix2x3.h>

using Matrix2x3 = sc::flash::Matrix2x3;

class DisplayObject
{
public:
    Matrix2x3 Matrix;
public:
    DisplayObject(/* args */);
    ~DisplayObject();
    void setX(float);
    void setY(float);
    void setXY(float, float);
    virtual bool render(Matrix2x3*);

};