#pragma once
class PopupBase;
#include "flash/Sprite.h"
#include "flash/MovieClip.h"

class GUI :public Sprite {
    Sprite* layers[7];
public:
    GUI();
    ~GUI();
    static GUI* sm_pInstance;
    static GUI* getInstance();
    static void constructInstance();
    static void destructInstance();
    void showPopup(PopupBase*);
    static void moveToScreenLeft(DisplayObject*);
    static void moveToScreenRight(DisplayObject*);
    static void scaleXToScreenWidth(MovieClip*, const char*, bool);
};