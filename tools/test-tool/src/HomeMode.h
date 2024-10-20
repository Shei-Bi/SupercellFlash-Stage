#pragma once
#include <GameState.h>

class HomeMode :public GameState
{
public:
    HomeMode();
    void update(float, float);
    void updateLoading(float);
    void enter();
};
