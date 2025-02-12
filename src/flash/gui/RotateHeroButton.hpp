#pragma once
#include "GameButton.h"
#include "flash/MovieClip.h"
#include "flash/3d/SceneRenderer.hpp"

class RotateHeroButton :public GameButton {
    SceneRenderer* heroSprite;
    bool b344;
    float f352, f356, f360;
    int i384;
    float f368[4];
public:
    RotateHeroButton(MovieClip* movieClip) :GameButton() {
        setMovieClip(movieClip, true);
        heroSprite = nullptr;
        b344 = false;
        f352 = f356 = f360 = 0.0f;
        i384 = 0;

    }
    void setHeroSprite(SceneRenderer* sceneRenderer) {
        heroSprite = sceneRenderer;
    }
    void update(float deltaTime) {
        // *(this + 388) = 0;
        if (b344)
        {
            int v4 = f356;
            int v5 = f360;
            int v6 = i384;
            f360 = v4;
            int v7 = v4 - v5;
            f368[v6] = v7;
            int v8 = v6 + 1;
            int v9 = f352 + v7;
            // v10 = *(this + 388);
            int v11;
            if (v6 + 1 >= 0)
                v11 = v6 + 1;
            else
                v11 = v6 + 4;
            i384 = v8 - (v11 & 0xFFFFFFFC);
        LABEL_7:
            f352 = v9;
            // goto LABEL_23;
        }
        // *(this + 368) = 0LL;
        // *(this + 376) = 0LL;
        // v10 = 0.0;
        while (f352 >= 360.0f) f352 -= 360.0f;
        while (f352 < 0.0f) f352 += 360.0f;
        heroSprite->character->yaw = f352;
        printf("f352: %f\n", f352);
    }
    bool touchPressed(Touch& touch) {
        b344 = false;
        return GameButton::touchPressed(touch);
    }
    bool touchMoved(Touch& touch) {
        if (!b344 && fabs(touch.x - touch.initialX) > 10.0f) {
            f360 = touch.x + ((touch.x - touch.initialX <= 10.0f) ? 10.0f : -10.0f);
            b344 = true;
        }
        if (b344) {
            f356 = touch.x;
        }
        // heroSprite->character->yaw = fabs(touch.x - touch.initialX);
        // heroSprite->character->pitch = fabs(touch.y - touch.initialY);
        return GameButton::touchMoved(touch);
    }
};