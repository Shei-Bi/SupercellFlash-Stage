#pragma once
#include <GameState.h>
#include <flash/MovieClip.h>
#include <flash/TextField.h>

class LoadingScreen :public GameState
{
public:
    // LoadingScreen();
    void update(float, float);
    void enter();
    void exit();
    MovieClip* loading_screen;
    MovieClip* progress_bar;
    TextField* text;
    static void showOnlyLocalizedLogo(char*, MovieClip*);
    void updateLoadingProgress(float);
    void setAlpha(float);
    float alpha;
};
