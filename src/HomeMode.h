#pragma once
#include <GameState.h>
#include <flash/gui/HomeScreen.hpp>

class HomeMode :public GameState
{
private:
    int state;
    int resourcesToLoad;
    HomeScreen* homeScreen;
public:
    // HomeMode();
    bool isLoaded();
    void update(float, float);
    void updateLoading(float);
    void enter();
    bool loadNextResource();
    float getLoadingProgress();

    static HomeMode* getInstance();
    HomeScreen* getHomeScreen();
};
