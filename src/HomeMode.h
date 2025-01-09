#pragma once
#include <GameState.h>
class HomeScreen;

class HomeMode :public GameState
{
private:
    int state;
    int resourcesToLoad;
    HomeScreen* homeScreen;
public:
    ~HomeMode();
    bool isLoaded();
    void update(float, float);
    void updateLoading(float);
    void enter();
    bool loadNextResource();
    float getLoadingProgress();
    void exit();

    static HomeMode* getInstance();
    HomeScreen* getHomeScreen();
};
