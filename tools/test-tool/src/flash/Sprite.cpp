#pragma once
#include <flash/Sprite.h>

Sprite::Sprite(int initialCapacity) :DisplayObject() {
    children = nullptr;
    size = 0;
    capacity = 0;
    allocateMemory(initialCapacity);
}
void Sprite::allocateMemory(int newCapacity) {
    if (newCapacity > capacity) {
        DisplayObject** newArray = new DisplayObject * [newCapacity];
        if (children) delete(children);
        children = newArray;
        capacity = newCapacity;
    }
}
void Sprite::addChild(DisplayObject* child) {
    children[size++] = child;
}
bool Sprite::render() {
    for (int i = 0;i < size;i++) children[i]->render();
    return true;
}
Sprite::Sprite(/* args */)
{
}

Sprite::~Sprite()
{
}
