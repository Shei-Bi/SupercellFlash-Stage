#pragma once
#include <GameState.h>

class InitState :public GameState
{
public:
    // InitState();
    void update(float, float);
    bool isLoaded();
    float getLoadingProgress();
};
