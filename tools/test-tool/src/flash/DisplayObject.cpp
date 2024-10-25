#pragma once
#include <flash/DisplayObject.h>
DisplayObject::DisplayObject(/* args */)
{
    visible = true;
    renderConfig = 0;
}

DisplayObject::~DisplayObject()
{
}
bool DisplayObject::render(Matrix2x3* mat, ColorTransform* c, int, float deltaTime) {
    return true;
}
void DisplayObject::setX(float x) {
    Matrix.tx = x;
}
void DisplayObject::setY(float y) {
    Matrix.ty = y;
}
float DisplayObject::getX() {
    return Matrix.tx;
}
float DisplayObject::getY() {
    return Matrix.ty;
}
void DisplayObject::setXY(float x, float y) {
    Matrix.tx = x;
    Matrix.ty = y;
}
void DisplayObject::setPixelSnappedXY(float x, float y) {
    Matrix.tx = floorf(x);
    Matrix.ty = floorf(y);
}
void DisplayObject::setScale(float s) {
    Matrix.a = s;
    Matrix.d = s;
}
void DisplayObject::setScaleX(float s) {
    Matrix.a = s;
}
void DisplayObject::setScaleY(float s) {
    Matrix.d = s;
}
void DisplayObject::setBlendMode(int b) {
    renderConfig = renderConfig & 0xFFFFFC7F | (((b >> 7) & 7) << 7);
}
void DisplayObject::setAlpha(float a) {
    colorTransform.alpha = a * 255.0f;
}