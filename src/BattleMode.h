#pragma once
#include <GameState.h>
class BattleScreen;
class ClientInputManager;
class LogicBattleModeClient;

class BattleMode :public GameState
{
private:
    int state;
    int resourcesToLoad;
    BattleScreen* battleScreen;
public:
    LogicBattleModeClient* battleClient;
    ClientInputManager* inputManager;

    BattleMode();
    ~BattleMode();
    bool isLoaded();
    bool isInited();
    void update(float, float);
    void updateLoading(float);
    void enter();
    bool loadNextResource();
    float getLoadingProgress();

    static BattleMode* getInstance();
    BattleScreen* getBattleScreen();
    void setGameOverResult(int result);

    bool touchReleased(Touch& t);
};
