#pragma once
#include "GameButton.h"
#include "flash/MovieClip.h"
#include "flash/3d/SceneRenderer.hpp"

class RotateHeroButton :public GameButton {
    SceneRenderer* heroSprite;
    bool draggingStarted;
    float yaw, nowX, previousX;
    int movementArrayIndex;
    float movementArray[4];
    float yawDelta;
#ifdef HERO_ROTATE_PITCH
    bool b344y;
    float f352y, f356y, f360y;
    int i384y;
    float f368y[4];
#endif
public:
    RotateHeroButton(MovieClip* movieClip) :GameButton() {
        setMovieClip(movieClip, true);
        heroSprite = nullptr;
        draggingStarted = false;
        yaw = yawDelta = nowX = previousX = 0.0f;
        movementArrayIndex = 0;
#ifdef HERO_ROTATE_PITCH
        b344y = false;
        f352y = f356y = f360y = 0.0f;
        i384y = 0;
#endif
    }
    void setHeroSprite(SceneRenderer* sceneRenderer) {
        heroSprite = sceneRenderer;
    }
    void update(float deltaTime) {
        if (!touchIned) {
            if (draggingStarted) {
                draggingStarted = false;
                yawDelta = (movementArray[0] + movementArray[1] + movementArray[2] + movementArray[3]) / 4.0f;
            }
            else {
                yaw += yawDelta;
                yawDelta *= 0.75f;
            }
            // printf("yaw after not touchin: %f\n", yaw);
        }
        else {
            if (draggingStarted) {
                float deltaX = nowX - previousX;
                yaw += deltaX;
                movementArray[movementArrayIndex] = deltaX;
                movementArrayIndex++;
                movementArrayIndex %= 4;

                previousX = nowX;
            }
            // printf("yaw after touchin: %f\n", yaw);
        }
        while (yaw >= 360.0f) yaw -= 360.0f;
        while (yaw < 0.0f) yaw += 360.0f;
        heroSprite->character->yaw = yaw;

#ifdef HERO_ROTATE_PITCH
        if (b344y)
        {
            int v4 = f356y;
            int v5 = f360y;
            int v6 = i384y;
            f360y = v4;
            int v7 = v4 - v5;
            f368y[v6] = v7;
            int v8 = v6 + 1;
            int v9 = f352y + v7;
            // v10 = *(this + 388);
            int v11;
            if (v6 + 1 >= 0)
                v11 = v6 + 1;
            else
                v11 = v6 + 4;
            i384y = v8 - (v11 & 0xFFFFFFFC);
            // LABEL_7:
            f352y = v9;
            // goto LABEL_23;
        }
        // *(this + 368) = 0LL;
        // *(this + 376) = 0LL;
        // v10 = 0.0;
        while (f352y >= 360.0f) f352y -= 360.0f;
        while (f352y < 0.0f) f352y += 360.0f;
        heroSprite->character->pitch = f352y;
#endif
    }
    bool touchPressed(Touch& touch) {
        draggingStarted = false;
        yawDelta = 0.0f;
#ifdef HERO_ROTATE_PITCH
        b344y = false;
#endif
        return GameButton::touchPressed(touch);
    }
    bool touchMoved(Touch& touch) {
        if (!draggingStarted && fabs(touch.x - touch.initialX) > 10.0f) {
            previousX = touch.x + ((touch.x - touch.initialX <= 10.0f) ? 10.0f : -10.0f);
            draggingStarted = true;
        }
        if (draggingStarted) {
            nowX = touch.x;
        }

#ifdef HERO_ROTATE_PITCH
        if (!b344y && fabs(touch.y - touch.initialY) > 10.0f) {
            f360y = touch.y + ((touch.y - touch.initialY <= 10.0f) ? 10.0f : -10.0f);
            b344y = true;
        }
        if (b344y) {
            f356y = touch.y;
        }
#endif
        return GameButton::touchMoved(touch);
    }
};