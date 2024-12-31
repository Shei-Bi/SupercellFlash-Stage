#pragma once
#include <flash/Sprite.h>
#include "flash/Stage.h"

Sprite::Sprite(short initialCapacity) :DisplayObject() {
    interactable = false;
    children = nullptr;
    size = 0;
    capacity = 0;
    if (initialCapacity != -1) allocateMemory(initialCapacity);
}
void Sprite::allocateMemory(short newCapacity) {
    if (newCapacity > capacity) {
        DisplayObject** newArray = new DisplayObject * [newCapacity];
        if (children) delete[] children;
        children = newArray;
        capacity = newCapacity;
    }
}
void Sprite::addChild(DisplayObject* child) {
    addChildAt(child, size);
}
void Sprite::addChildAt(DisplayObject* child, short index) {
    if (child->parent == this) {
        int oldIndex = child->indexInParent;
        int newIndex;
        if (index >= size) {
            newIndex = size - 1;
        }
        else newIndex = index - oldIndex < index;
        if (newIndex == oldIndex) return;
        if (oldIndex < newIndex) {
            for (int i = oldIndex;i < newIndex;i++) {
                children[i] = children[i + 1];
                children[i]->indexInParent = i;
            }
        }
        else {
            for (int i = oldIndex;i > newIndex;i--) {
                children[i] = children[i - 1];
                children[i]->indexInParent = i;
            }
        }
        children[newIndex] = child;
        child->indexInParent = newIndex;
        return;
    }
    if (child->parent) {
        child->parent->removeChildAt(child->indexInParent);
    }
    if (size == capacity) {
        short newCapacity = capacity == 0 ? 1 : capacity * 2;
        DisplayObject** newArray = new DisplayObject * [newCapacity];
        if (children != nullptr) {
            for (int i = 0;i < size;i++) {
                newArray[i] = children[i];
            }
            delete[] children;
        }
        children = newArray;
        capacity = newCapacity;
    }
    if (index < size) {
        for (int i = size;i > index;i--) {
            children[i] = children[i - 1];
            children[i]->indexInParent = i;
        }
    }
    children[index] = child;
    child->parent = this;
    child->indexInParent = index;
    size++;
}
void Sprite::removeChildAt(short index) {
    children[index]->parent = nullptr;
    children[index]->indexInParent = -1;
    size--;
    if (index < size) {
        for (int i = index;i < size;i++) {
            children[i] = children[i + 1];
            children[i]->indexInParent = i;
        }
    }
    children[size] = nullptr;
}
void Sprite::removeChild(DisplayObject* child) {
    if (child->parent == this && child->indexInParent != -1) removeChildAt(child->indexInParent);
}
bool Sprite::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    Matrix2x3* n = new Matrix2x3(Matrix, *mat);
    ColorTransform* ct = new ColorTransform(colorTransform, *c);
    for (int i = 0;i < size;i++) if (children[i]->visible) children[i]->render(n, ct, renderConfig & 0x3FF | rc, deltaTime);
    delete n;
    delete ct;
    return true;
}
Sprite::Sprite() :Sprite(4) {
    ;
}

Sprite::~Sprite() {
    if (children) delete[] children;
    children = nullptr;
}
int Sprite::getChildIndex(DisplayObject* displayObject) {
    if (displayObject->parent == this) return displayObject->indexInParent;
    return -1;
}
bool Sprite::collisionRender(Matrix2x3* mat) {
    if (!interactable) return false;
    auto& sprites = Stage::getInstance()->objectsUnderPoint;
    sprites.push_back(this);
    Matrix2x3* n = new Matrix2x3(Matrix, *mat);
    bool result = false;
    for (int i = 0;i < size;i++) if (children[i]->visible) result |= children[i]->collisionRender(n);
    delete n;
    if (!result) sprites.pop_back();
    return result;
}
void Sprite::setInteractiveRecursive(bool b) {
    interactable = b;
    for (int i = 0;i < size;i++) {
        children[i]->setInteractiveRecursive(b);
    }
}
