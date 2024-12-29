#pragma once
#include "Touch.h"
class IInputListener {
public:
    virtual bool touchPressed(Touch&) { return false; }
    virtual bool touchMoved(Touch&) { return false; }
    virtual bool touchReleased(Touch&) { return false; }
};