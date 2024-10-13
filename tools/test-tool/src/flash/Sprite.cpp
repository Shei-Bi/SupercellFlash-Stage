#pragma once
#include <flash/Sprite.h>

Sprite::Sprite(short initialCapacity) :DisplayObject() {
    children = nullptr;
    size = 0;
    capacity = 0;
    if (initialCapacity != -1) allocateMemory(initialCapacity);
}
void Sprite::allocateMemory(short newCapacity) {
    if (newCapacity > capacity) {
        DisplayObject** newArray = new DisplayObject * [newCapacity];
        if (children) delete(children);
        children = newArray;
        capacity = newCapacity;
    }
}
void Sprite::addChild(DisplayObject* child) {
    addChildAt(child, size);
}
void Sprite::addChildAt(DisplayObject* child, short index) {
    if (size == capacity) {
        short newCapacity = capacity == 0 ? 1 : capacity * 2;
        DisplayObject** newArray = new DisplayObject * [newCapacity];
        if (children != nullptr) {
            for (int i = 0;i < size;i++) {
                newArray[i] = children[i];
            }
            delete(children);
        }
        children = newArray;
        capacity = newCapacity;
    }
    if (index < size) {
        for (int i = size;i > index;i--) {
            children[i] = children[i - 1];
        }
    }
    children[index] = child;
    size++;
}
void Sprite::removeChildAt(short index) {
    size--;
    if (index < size) {
        for (int i = index;i < size;i++) {
            children[i] = children[i + 1];
        }
    }
    children[size] = nullptr;
}
bool Sprite::render(Matrix2x3* mat, ColorTransform* c, float deltaTime) {
    Matrix2x3* n = new Matrix2x3(Matrix);
    n->multiply(mat);
    ColorTransform* ct = new ColorTransform(colorTransform);
    ct->multiplyy(c);
    for (int i = 0;i < size;i++) children[i]->render(n, ct, deltaTime);
    return true;
}
Sprite::Sprite(/* args */)
{
}

Sprite::~Sprite()
{
}
