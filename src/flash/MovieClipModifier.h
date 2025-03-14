#pragma once
#include <flash/DisplayObject.h>
class MovieClipModifierOriginal;

class MovieClipModifier :public DisplayObject
{
public:
    int type;
    MovieClipModifier(MovieClipModifierOriginal* original);
    bool render(Matrix2x3* mat, ColorTransform* c, int, float);
    bool collisionRender(Matrix2x3* mat);
};