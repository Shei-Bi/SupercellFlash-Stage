#pragma once
#include "IInputListener.h"

class GameState :public IInputListener
{
public:
    virtual ~GameState();
    virtual float getLoadingProgress();
    virtual void enter();
    virtual void exit();
    virtual void updateLoading(float);
    virtual bool isLoaded();
    virtual bool isInited();
    virtual void setAlpha(float);
    virtual void update(float, float);
};