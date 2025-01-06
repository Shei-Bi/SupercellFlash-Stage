#pragma once
#include <GameState.h>
#include <logic/LogicClientHome.hpp>
#include <logic/LogicClientAvatar.hpp>

class GameStateManager
{
public:
    enum StateId
    {
        Null = 0,
        Init = 2,
        Loading = 3,
        Home = 4,
        Battle = 5,
        MapEditor = 6
    };
    GameStateManager();
    static GameStateManager* sm_pInstance;
    static GameStateManager* getInstance();
    static void constructInstance();
    GameState* createState(StateId);
    void init();
    void update(float, float);
    void draw(float);
    bool isChangingMode();
    bool hasGameData();
    void setGameData(LogicClientHome*, LogicClientAvatar*);
    bool isState(StateId);

    // void createState(StateId);
    GameState* loadingScreen;
    GameState* currentState;
    StateId currentStateId;
    StateId pendingStateId;
    float intervalTime;
    LogicClientHome* home;
    LogicClientAvatar* avatar;
    int gameDataLoaded;
};
