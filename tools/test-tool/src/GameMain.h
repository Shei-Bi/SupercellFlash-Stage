#pragma once

class GameMain
{
public:
    GameMain();
    static GameMain* sm_pInstance;
    static GameMain* getInstance();
    static void constructInstance();
    void init();
    void update(float, float);
    void draw(float);
    float deltaTime;
};
