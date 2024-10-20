#pragma once
#include <GameState.h>

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
    // void createState(StateId);
    GameState* loadingScreen;
    GameState* currentState;
    StateId currentStateId;
    StateId pendingStateId;
    float intervalTime;
};
