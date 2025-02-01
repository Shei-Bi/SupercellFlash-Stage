#pragma once
#include <flash/DisplayObject.h>

class TextFieldOriginal;

class TextField :public DisplayObject
{
private:
    /* data */
public:
    Rect bounds;
    bool render(Matrix2x3*, ColorTransform* c, int, float);
    static TextField* createTextField(TextFieldOriginal* original);
};