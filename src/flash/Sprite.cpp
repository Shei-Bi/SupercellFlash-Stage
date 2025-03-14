#pragma once
#include <flash/Sprite.h>
#include "flash/Stage.h"
#include "Sprite.h"
#include "MovieClip.h"
#include "ResourceManager.h"

Sprite::Sprite(short initialCapacity) :DisplayObject() {
    interactable = false;
    children = nullptr;
    size = 0;
    capacity = 0;
    hitArea = nullptr;
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
void Sprite::removeAllChildren() {
    for (short i = size - 1;i >= 0;i--) removeChildAt(i);
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
    if (hitArea) delete hitArea;
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
    bool result = hitArea ? hitAreaTest(n) : false;
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

void Sprite::setHitArea(Rect* hitArea) {
    if (this->hitArea != hitArea) {
        if (this->hitArea) delete this->hitArea;
        this->hitArea = hitArea;
    }
}

bool Sprite::hitAreaTest(Matrix2x3* mat) {
    float x1 = mat->applyX(hitArea->left, hitArea->top);
    float y1 = mat->applyY(hitArea->left, hitArea->top);
    float x2 = mat->applyX(hitArea->right, hitArea->top);
    float y2 = mat->applyY(hitArea->right, hitArea->top);
    float x3 = mat->applyX(hitArea->right, hitArea->bottom);
    float y3 = mat->applyY(hitArea->right, hitArea->bottom);
    float x4 = mat->applyX(hitArea->left, hitArea->bottom);
    float y4 = mat->applyY(hitArea->left, hitArea->bottom);
    float left = fmin(fmin(fmin(x1, x2), x3), x4);
    float right = fmax(fmax(fmax(x1, x2), x3), x4);
    float top = fmin(fmin(fmin(y1, y2), y3), y4);
    float bottom = fmax(fmax(fmax(y1, y2), y3), y4);

    return Stage::getInstance()->isRectVisibleInTouch(left, top, right, bottom);
}

MovieClip* Sprite::createScreenContainer(const char* name, int index) {
    std::string s("");
    float x = Stage::getInstance()->matrixX;
    float y = Stage::getInstance()->matrixY;
    switch (index) {
    case 0:
        s = "bg";
        x *= 0.5f;
        y *= 0.5f;
        break;
    case 1:
        s = "center";
        x *= 0.5f;
        y *= 0.5f;
        break;
    case 2:
        s = "hud_top";
        x *= 0.5f;
        y = 0.0f;
        break;
    case 3:
        s = "hud_bottom";
        x *= 0.5f;
        break;
    case 4:
        s = "hud_bottom_right";
        break;
    case 5:
        s = "hud_left";
        x = 0.0f;
        y *= 0.5f;
        break;
    case 6:
        s = "hud_right";
        y *= 0.5f;
        break;
    case 7:
        s = "hud_top_left";
        x = 0.0f;
        y = 0.0f;
        break;
    case 8:
        s = "hud_top_right";
        y = 0.0f;
        break;
    case 9:
        s = "hud_bottom_left";
        x = 0.0f;
        break;
    }
    SupercellSWF* supercellSWF = ResourceManager::getSupercellSWF("sc/ui.sc", nullptr);

    MovieClip* c = nullptr;
    std::string nameString(name);
    if (!supercellSWF->hasExportName((char*)nameString.c_str()))
        nameString += s;

    if (supercellSWF->hasExportName((char*)nameString.c_str())) {
        c = ResourceManager::getMovieClip("sc/ui.sc", (char*)nameString.c_str());
        addChild(c);
        c->setPixelSnappedXY(x, y);
        c->setInteractiveRecursive(true);
    }
    return c;
}
void Sprite::addScreenContainer(const char* name, int index, std::vector<MovieClip*>& vector) {
    auto c = createScreenContainer(name, index);
    if (c) vector[index] = c;
}
void Sprite::initScreenContainers(const char* name, std::vector<MovieClip*>& vector) {
    for (int i = 0;i < 10;i++) {
        vector.push_back(createScreenContainer(name, i));
    }
}

int Sprite::whyUninteractable() {
    int i = 0;
    auto p = this;
    while (p && p->interactable) {
        i++;
        p = p->parent;
    }
    return i;
}
