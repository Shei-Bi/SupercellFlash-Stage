#pragma once
#include <GameState.h>

class InitState :public GameState
{
public:
    int state;
    // InitState();
    void enter();
    void updateLoading(float);
    void update(float, float);
    bool isLoaded();
    float getLoadingProgress();
    void initDataTables();
};
