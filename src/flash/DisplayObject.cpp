#pragma once
#include <flash/DisplayObject.h>
#include <flash/Sprite.h>
#include <flash/Stage.h>

DisplayObject::DisplayObject(/* args */)
{
    visible = true;
    renderConfig = 0;
    parent = nullptr;
    indexInParent = -1;
}

DisplayObject::~DisplayObject() {
    if (parent) parent->removeChildAt(indexInParent);
}
bool DisplayObject::render(Matrix2x3* mat, ColorTransform* c, int, float deltaTime) {
    return true;
}
bool DisplayObject::collisionRender(Matrix2x3* mat) {
    return false;
}
void DisplayObject::setInteractiveRecursive(bool b) {
    ;
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
float DisplayObject::getScaleX() {
    return Matrix.a;
}
float DisplayObject::getScaleY() {
    return Matrix.d;
}
void DisplayObject::setBlendMode(int b) {
    renderConfig = renderConfig & 0xFFFFFC7F | (((b >> 7) & 7) << 7);
}
void DisplayObject::setAlpha(float a) {
    colorTransform.alpha = a * 255.0f;
}
float DisplayObject::getWidth() {
    Rect rect;
    bool visible = this->visible;
    Stage::getInstance()->calculateDisplayObjectBounds(this, nullptr, &rect);
    this->visible = visible;
    return rect.right - rect.left;
}
float DisplayObject::getHeight() {
    Rect rect;
    bool visible = this->visible;
    Stage::getInstance()->calculateDisplayObjectBounds(this, nullptr, &rect);
    this->visible = visible;
    return rect.bottom - rect.top;
}