#include <flash/gui/GUI.h>
#include "GUI.h"
#include "PopupBase.hpp"
#include "flash/Stage.h"

GUI* GUI::sm_pInstance = nullptr;
GUI* GUI::getInstance() {
    return sm_pInstance;
}
GUI::GUI() :Sprite(7) {
    interactable = true;
    for (int i = 0;i < 7;i++) {
        layers[i] = new Sprite(8);
        layers[i]->interactable = true;
        addChild(layers[i]);
    }
}
GUI::~GUI() {
    for (int i = 0;i < 7;i++) {
        delete layers[i];
    }
}
void GUI::constructInstance() {
    if (!GUI::sm_pInstance)
        GUI::sm_pInstance = new GUI();
}
void GUI::destructInstance() {
    if (GUI::sm_pInstance) delete GUI::sm_pInstance;
    GUI::sm_pInstance = nullptr;
}
void GUI::showPopup(PopupBase* popup) {
    layers[2]->addChild(popup);
}

void GUI::moveToScreenLeft(DisplayObject* target) {
    target->setPixelSnappedXY(target->getX() + 512.0f - Stage::getInstance()->matrixX * 0.5f, target->getY());
}

void GUI::moveToScreenRight(DisplayObject* target) {
    target->setPixelSnappedXY(target->getX() - 512.0f + Stage::getInstance()->matrixX * 0.5f, target->getY());
}

void GUI::scaleXToScreenWidth(MovieClip* target, const char* childName, bool considerSafeMargin) {
    auto child = target->getChildByName(childName);
    child->setScaleX(Stage::getInstance()->matrixX / child->getWidth() * child->getScaleX());
}
