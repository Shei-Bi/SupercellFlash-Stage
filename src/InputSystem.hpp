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
    InputSystem() {
        leftBtnState = GLFW_RELEASE;
    }
    void update() {
        int state = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT);
        Touch touch;
        double xpos, ypos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        touch.x = xpos;
        touch.y = ypos;
        if (leftBtnState == GLFW_RELEASE && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
            printf("pressed %f   %f\n", touch.x, touch.y);
            for (IInputListener* i : listeners) if (i->touchPressed(touch)) break;
        }
        else if (leftBtnState == GLFW_PRESS || leftBtnState == GLFW_REPEAT) {
            if (state == GLFW_PRESS || state == GLFW_REPEAT) {
                printf("moved %f   %f\n", touch.x, touch.y);
                for (IInputListener* i : listeners) if (i->touchMoved(touch)) break;
            }
            else if (state == GLFW_RELEASE) {
                printf("released %f   %f\n", touch.x, touch.y);
                for (IInputListener* i : listeners) if (i->touchReleased(touch)) break;
            }
        }
        leftBtnState = state;
        // ;glfwGetCursorPos(window, &xpos, &ypos);
    }
};