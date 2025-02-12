#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "stdio.h"
#include "Touch.h"
#include "IInputListener.h"
#include <vector>
class InputSystem {
public:
    int leftBtnState;
    std::vector<IInputListener*> listeners;
    Touch currentTouch;
    InputSystem() {
        leftBtnState = GLFW_RELEASE;
    }
    void update() {
        int state = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT);
        double xpos, ypos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        if (leftBtnState == GLFW_RELEASE && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
            currentTouch = Touch();
            currentTouch.x = (float)xpos;
            currentTouch.y = (float)ypos;
            currentTouch.initialX = (float)xpos;
            currentTouch.initialY = (float)ypos;
            for (IInputListener* i : listeners) if (i->touchPressed(currentTouch)) break;
        }
        else if (leftBtnState == GLFW_PRESS || leftBtnState == GLFW_REPEAT) {
            if (state == GLFW_PRESS || state == GLFW_REPEAT) {
                if (currentTouch.x != (float)xpos || currentTouch.y != (float)ypos) {
                    currentTouch.previousX = currentTouch.x;
                    currentTouch.previousY = currentTouch.y;
                    currentTouch.x = (float)xpos;
                    currentTouch.y = (float)ypos;
                    for (IInputListener* i : listeners) if (i->touchMoved(currentTouch)) break;
                }
            }
            else if (state == GLFW_RELEASE) {
                // printf("released %f   %f\n", touch.x, touch.y);
                for (IInputListener* i : listeners) if (i->touchReleased(currentTouch)) break;
            }
        }
        leftBtnState = state;
        // ;glfwGetCursorPos(window, &xpos, &ypos);
    }
};