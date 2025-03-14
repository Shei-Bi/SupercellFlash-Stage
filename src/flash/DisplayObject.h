#pragma once
#include "Matrix2x3.h"
#include "ColorTransform.h"
#include "Rect.h"

class Sprite;

class DisplayObject
{
public:
    ColorTransform colorTransform;
    bool visible;
    Matrix2x3 Matrix;
    int renderConfig;
    Sprite* parent;
    int indexInParent;
#ifdef MOVIECLIP_DEBUG
    const char* name;
#endif
public:
    DisplayObject(/* args */);
    virtual ~DisplayObject();
    void setX(float);
    void setY(float);
    float getX();
    float getY();
    void setXY(float, float);
    void setPixelSnappedXY(float, float);
    void setScale(float);
    void setScaleX(float);
    void setScaleY(float);
    float getScaleX();
    float getScaleY();
    virtual bool render(Matrix2x3*, ColorTransform*, int, float);
    virtual bool collisionRender(Matrix2x3*);
    virtual void setInteractiveRecursive(bool);
    void setBlendMode(int);
    void setAlpha(float);
    float getWidth();
    float getHeight();
    void getGlobalBounds(Rect* boundsOut, bool);

    virtual bool isShape() const { return false; };
    virtual bool isMovieClip() const { return false; };
    virtual bool isMovieClipModifier() const { return false; };
    virtual bool isTextfield() const { return false; };
};