#pragma once
#include "flash/MovieClip.h"

class ScrollArea :public Sprite {
public:
    Sprite* container;
    float width, height;
    ScrollArea(float width, float height, int capacity) : Sprite(1) {
        init(width, height, capacity);
    }
    void init(float width, float height, int capacity) {
        this->width = width;
        this->height = height;
        interactable = true;
        setHitArea(new Rect(0, 0, width, height));
        container = new Sprite(capacity);
        container->interactable = true;
        addChild(container);
    }
    void removeAllContent() {
        container->removeAllChildren();
    }
    void addContentDontUpdateBounds(DisplayObject* content) {
        container->addChild(content);
    }
};