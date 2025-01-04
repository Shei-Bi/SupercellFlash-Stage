#pragma once
#include <flash/Sprite.h>
#include "InputSystem.hpp"
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
    Sprite* screen1;
    Sprite* screen2;
    Sprite* screen3;
    Sprite* screen4;

    InputSystem* inputSystem;
};
