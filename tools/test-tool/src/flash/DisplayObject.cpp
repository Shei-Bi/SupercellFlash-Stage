#pragma once
#include <flash/DisplayObject.h>
DisplayObject::DisplayObject(/* args */)
{
}

DisplayObject::~DisplayObject()
{
}
bool DisplayObject::render(Matrix2x3* mat, ColorTransform* c, float deltaTime) {
    return true;
}
void DisplayObject::setX(float x) {
    Matrix.tx = x;
}
void DisplayObject::setY(float y) {
    Matrix.ty = y;
}
void DisplayObject::setXY(float x, float y) {
    Matrix.tx = x;
    Matrix.ty = y;
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